#include "../include/http_tcp_linux.hpp"
#include <iostream>

int main(int argc, char * argv[]){

    if (argc == 2){
        http::TcpServer server = http::TcpServer(std::string(argv[1]));
        server.startServer();
    }
    else if (argc == 3){

        http::TcpServer server = http::TcpServer(std::string(argv[1]), std::stoi(argv[2]));
        server.startServer();
    }
    else{
        perror("http_tcp_linux <ip address> <port>");
        exit(1);
    }

}