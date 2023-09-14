#include "../include/gooseDB.h"
#include <string>

int main(int argc, char * argv[]){
    gooseDB::GooseServer server1;

    if (argc == 2){
        server1.startServer(std::string(argv[1]));
    }
    else if (argc == 3){
        server1.startServer(std::string(argv[1]), std::stoi(argv[2]));
    }
    else{
        perror("gooserDB_server1 <ip address> <port>");
        exit(1);
    }
}