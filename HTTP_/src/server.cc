#include <iostream>

#include "../include/http_tcp_linux.h"

void message_handler(char incoming_message[http::BUFFER_SIZE], http::TcpServer * server){
    server->send_response("");
}






int main(int argc, char * argv[]){

    if (argc == 2){
        http::TcpServer server = http::TcpServer(std::string(argv[1]));
        server.startServer();
    }
    else if (argc == 3){

        http::TcpServer server = http::TcpServer(std::string(argv[1]), std::stoi(argv[2]));
        server.setup_handler(&message_handler);
        server.startServer();
    }
    else{
        perror("http_tcp_linux <ip address> <port>");
        exit(1);
    }

}