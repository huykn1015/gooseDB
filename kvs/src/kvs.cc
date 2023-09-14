#include "../include/kvs.h"

#include <mach-o/dyld.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <libproc.h>
#include <unistd.h>
#include <sys/stat.h>

#include <iostream>
#include <fstream>
#include <sstream>

namespace kvs{

    const size_t token_length = 16;

    std::string hash(std::string key, int divisor){
        std::hash<std::string> hash_obj;
        return std::to_string(static_cast<unsigned int>(hash_obj(key)) % divisor);
    }

    std::string getPath(){
        char buffer[1000];
        getcwd(buffer, sizeof(buffer));
        return std::string(buffer);
    }

    bool KeyValueStore::fileExists(std::string filename){
        std::string fullpath = prog_path + filename;
        struct stat sb;
        if (stat(fullpath.c_str(), &sb) == 0)
            return true;
        return false;

    }

    char randChar(){
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    }

    std::string generate_token( size_t length ) {
        std::string str(length,0);
        std::generate_n(str.begin(), length, randChar);
        return str;
    }

    int generateInfoFile(std::string name, std::string &edit_token, std::string &read_token, int kvs_size){
        std::ofstream info_file(name + ".txt");
        info_file << kvs_size << std::endl;
        info_file << "0" << std::endl;
        edit_token = generate_token(token_length);
        read_token = generate_token(token_length);
        info_file << edit_token << std::endl << read_token;
    }

    KeyValueStore::KeyValueStore(){
        prog_path = getPath() + "/";
        state = NONE;
    }

    KeyValueStore::~KeyValueStore(){
    }

    int KeyValueStore::create_kvs(std::string name, std::string &edit_token, std::string &read_token, int kvs_size){
        std::string fullpath = prog_path + name;
        if(fileExists(name)){
            std::cout<< "DB already exists";
            return 1;
        }
        if(mkdir(name.c_str(),0777)){
            std::cout<< "Error creating DB";
            return 1;
        }
        chdir((fullpath).c_str());
        generateInfoFile(name, edit_token, read_token, kvs_size);
        std::cout<< "DB " << name << " successfully created" << std::endl;
        chdir(prog_path.c_str());
        return 0;
    }

    int KeyValueStore::open_kvs(std::string name, std::string token){
        db_path = prog_path + name;
        if(!fileExists(name)){
            std::cout<< db_path <<" does not exists\n";
            std::cout << "Cur Path: " << getPath() << std::endl;
            db_path = "";
            return 1;
        }
        chdir((db_path).c_str());
        db_name = name;
        getDBInfo();
        if(token == open_db.read_token){
            std::cout << name << " opened in read mode\n";
            state = READ;
        }
        else if(token == open_db.edit_token){
            std::cout << name << " opened in write mode\n";
            state = EDIT;
        }
        else{
            resetOpenDB();
            return 1;
        }
        return 0;
    }

    void KeyValueStore::close_kvs(){
        resetOpenDB();
        chdir(prog_path.c_str());
    }

    int KeyValueStore::add_entry(std::string key, std::string value){
        if(state != EDIT){
            std::cout << "ADD: Incorrect permissions";
            return 1;
        }
        std::string hashed_key = hash(key, open_db.kvs_size);
        std::string hash_folder_path = db_path + "/" + hashed_key;
        if(!fileExists(hash_folder_path)){
            mkdir(hashed_key.c_str(), 0777);
        }
        std::ofstream entry_file;
        entry_file.open(hash_folder_path + "/" + key);
        entry_file << value;
        entry_file.close();
        return 0;
    }

    int KeyValueStore::get_entry(std::string key, std::string &buffer){
        if(state < EDIT){
            std::cout << "GET: Incorrect Permissions";

            std::cout<< std::to_string(state) <<std::endl;
            return 1;
        }
        std::string hashed_key = hash(key, open_db.kvs_size);
        std::string entry_path = db_name + "/" + hashed_key + "/" + key;
        if(!fileExists(entry_path)){
            std::cout<< "GET: Entry does not exist\n";
            return 1;
        }
        std::ifstream entry_file(prog_path + entry_path);
        std::ostringstream ss;
        ss << entry_file.rdbuf();
        buffer = ss.str();
        return 0;
    }

    int KeyValueStore::update_entry(std::string key, std::string new_value){
        if(state != EDIT){
            std::cout << "UPDATE: Incorrect permissions";
            return 1;
        }
        std::string hashed_key = hash(key, open_db.kvs_size);
        std::string entry_path =  db_name + "/" + hashed_key + "/" + key;
        if(!fileExists(entry_path)){
            std::cout<< "UPDATE: Entry does not exist\n";
            return 1;
        }
        std::ofstream entry_file;
        entry_file.open(prog_path + entry_path);
        entry_file << new_value;
        entry_file.close();
        return 0;
    }

    int KeyValueStore::delete_entry(std::string key){
        if(state != EDIT){
            std::cout << "DELETE: Incorrect permissions";
            return 1;
        }
        std::string hashed_key = hash(key, open_db.kvs_size);
        std::string entry_path = db_name + "/" + hashed_key + "/" + key;
        if(!fileExists(entry_path)){
            std::cout<< "DELETE: Entry does not exist";
            return 1;
        }
        if(remove((prog_path + entry_path).c_str())){
            return 1;
        }
        return 0;
    }

    int KeyValueStore::getDBInfo(){
        std::ifstream info_file;
        info_file.open(db_name + ".txt");
        std::string buffer;
        std::getline(info_file, buffer);
        open_db.kvs_size = std::stoi(buffer);
        std::getline(info_file, buffer);
        open_db.entries = std::stoi(buffer);
        std::getline(info_file, open_db.edit_token);
        std::getline(info_file, open_db.read_token);
        return 0;
    }

    int KeyValueStore::updateInfoFile(){
        std::ofstream info_file;
        info_file.open(db_name + ".txt", std::ofstream::trunc);
        info_file << std::to_string(open_db.kvs_size) << std::endl;
        info_file << std::to_string(open_db.entries) << std::endl;
        info_file << open_db.read_token << std::endl << open_db.read_token;
        return 0;
    }


}