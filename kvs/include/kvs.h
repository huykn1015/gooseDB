#ifndef INCLUDED_KVS
#define INCLUDED_KVS

#include <string>

namespace kvs{
    const int DEFAULT_KVS_SIZE = 4096;
    enum dbState {NONE, EDIT, READ}; // None if no db open, EDIT if edit token given, Read if read token given
    class KeyValueStore{
    public:
        KeyValueStore(); 
        ~KeyValueStore();
        int create_kvs(std::string name, std::string &edit_token, std::string &read_token, int kvs_size = DEFAULT_KVS_SIZE); // creates db and writes tokens into strings
        int open_kvs(std::string name); // opens dir for kvs obj if exists
        int add_entry(std::string key, std::string value); // add entry to kvs
        std::string get_entry(std::string key); // returns string of kvs
        int update_entry(std::string key, std::string new_value); // changes value of an extry
        int delete_entry(std::string key); // removes entry from kvs
        void close_kvs(); // closes dir for kvs obj


    private:
        std::string edit_token;
        std::string read_token;
        std::string db_name;
        std::string db_path;
        enum dbState state;
        int kvs_size;
        int entries;
        int db_fd;
        bool fileExists(std::string name);

    };
};










#endif