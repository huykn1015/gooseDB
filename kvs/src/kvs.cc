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

namespace kvs{

    const size_t token_length = 16;


    int hash(std::string key, int divisor){
        std::hash<std::string> hash_obj;
        return static_cast<unsigned int>(hash_obj(key)) % divisor;
    }


    std::string getPath(){
        char buffer[1000];
        getcwd(buffer, sizeof(buffer));
        return std::string(buffer);
    }

    bool KeyValueStore::fileExists(std::string filename){
        std::string fullpath = db_path + filename;
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


    KeyValueStore::KeyValueStore(){
        db_path = getPath();
    }
    
    KeyValueStore::~KeyValueStore(){

    }

    int KeyValueStore::create_kvs(std::string name, std::string &edit_token, std::string &read_token, int kvs_size){
        std::string fullpath = db_path + name;
        if(fileExists(fullpath)){
            std::cout<< "DB already exists";
            return 1;
        }
        if(mkdir(name.c_str(),0777)){
            std::cout<< "Error creating DB";
            return 1;
        }
        chdir((db_path + "/" + name).c_str());
        std::ofstream info_file(name + ".txt");
        info_file << name << std::endl;
        info_file << kvs_size << std::endl;
        info_file << "0" << std::endl;
        edit_token = generate_token(token_length);
        read_token = generate_token(token_length);
        info_file << edit_token << std::endl << read_token;
        std::cout<< "DB " << name << "successfully created" << std::endl;
        return 0;
    }

    int KeyValueStore::open_kvs(std::string name){
        std::string fullpath = db_path + name;
        if(!fileExists(fullpath)){
            std::cout<< "DB does not exists";
            return 1;
        }
        chdir((db_path + "/" + name).c_str());






        return 0;
    }

    int KeyValueStore::add_entry(std::string key, std::string value){
        return 0;
    }

    std::string KeyValueStore::get_entry(std::string key){
        return NULL;
    }

    int KeyValueStore::update_entry(std::string key, std::string new_value){
        return 0;
    }




}