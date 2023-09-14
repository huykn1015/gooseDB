#include "../include/kvs.h"
#include <iostream>

int main (int argc, char* argv[]) {
    kvs::KeyValueStore test_db;
    std::string edit_key;
    std::string read_key;
    std::string buffer;


    // testdb "testdb1"
    test_db.create_kvs("testdb1", edit_key, read_key);
    test_db.open_kvs("testdb1", edit_key);
    test_db.add_entry("hello", "hi");
    test_db.add_entry("test", "entry");
    test_db.get_entry("test", buffer);
    std::cout<< "Buffer: " << buffer << std::endl;
    test_db.add_entry("test", "entry2");
    test_db.get_entry("test", buffer);
    std::cout<< "Buffer: " << buffer << std::endl;
    test_db.delete_entry("hello");
    test_db.close_kvs();

    test_db.open_kvs("testdb1", read_key);
    test_db.get_entry("test", buffer);
    std::cout<< "Buffer: " << buffer << std::endl;
    test_db.add_entry("hello", "hi");
    test_db.close_kvs();
}


/*
int main(){
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0)
        printf("executable path is %s\n", path);
    else
        printf("buffer too small; need size %u\n", size);
}

*/