#ifndef INCLUDED_KVS
#define INCLUDED_KVS
#include <string>

class kvs{
public:
    kvs();
    ~kvs();
    int create_kvs(std::string name);
    int open_kvs(std::string name);
    int add_entry(std::string key, std::string value);
    std::string get_entry(std::string key);
    int update_entry(std::string key, std::string new_value);
    int delete_entry(std::string key);
    void close_kvs();


private:
    std::string edit_token;
    std::string read_token;
    std::string db_name;    
    std::string db_path;
    int entries;

};









#endif