#ifndef INCLUDED_KVS
#define INCLUDED_KVS

#include <string>
#include<iostream>
#include <experimental/filesystem>

namespace kvs{
    const int DEFAULT_KVS_SIZE = 4096;
    enum dbState {NONE, EDIT, READ}; // None if no db open, EDIT if edit token given, Read if read token given
    
    struct db_info {
        int kvs_size;
        int entries;
        std::string edit_token;
        std::string read_token;
    };
    
    
    class KeyValueStore{
    public:
        KeyValueStore(); 
        ~KeyValueStore();
        int create_kvs(std::string name, std::string &edit_token, std::string &read_token, int kvs_size = DEFAULT_KVS_SIZE); // creates db and writes tokens into strings
        int open_kvs(std::string name, std::string token); // opens dir for kvs obj if exists
        int add_entry(std::string key, std::string value); // add entry to kvs
        int get_entry(std::string key, std::string &buffer); // returns string of kvs
        int update_entry(std::string key, std::string new_value); // changes value of an extry
        int delete_entry(std::string key); // removes entry from kvs
        int checkEditToken(std::string token){
            return token == open_db.edit_token;
        }
        int delete_db(){
            std::string delete_name = db_name;
            close_kvs();
            std::filesystem::remove_all(delete_name);
            return 0;
        }
        void close_kvs(); // closes dir for kvs obj
        bool checkReadStatus(){
            return state >= EDIT;
        }
        bool checkEditStatus(){
            return state == EDIT;
        }

    private:
        std::string prog_path;
        std::string db_path;
        std::string db_name;
        struct db_info open_db;
        enum dbState state;
        bool fileExists(std::string name);
        int getDBInfo();
        int updateInfoFile();

        void resetOpenDB(){
            db_name = "";
            db_path = "";
            state = NONE;
            open_db.kvs_size = 0;
            open_db.entries = 0;
            open_db.edit_token = "";
            open_db.read_token = "";
        }
    };
};










#endif