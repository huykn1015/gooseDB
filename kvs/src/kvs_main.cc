#include "../include/kvs.h"

int main (int argc, char* argv[]) {
    kvs::KeyValueStore test_db;
    std::string edit_key;
    std::string read_key;
    test_db.create_kvs("testdb", edit_key, read_key);

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