#include <iostream>

#include "../include/http_tcp_linux.h"

class Test_handler:public http::TcpServer::HTTP_handler{
public:
    Test_handler(){}
    ~Test_handler(){}
private:
    void message_handler(char incoming_message[http::BUFFER_SIZE], http::TcpServer * server){
        std::cout<<"idk\n";
        server->send_response(http::create_response_("Hello World!"));
    }
};





int main(int argc, char * argv[]){
    Test_handler test;
    if (argc == 2){
        http::TcpServer server = http::TcpServer(std::string(argv[1]));
        server.setup_handler(&test);
        server.startServer();
    }
    else if (argc == 3){

        http::TcpServer server = http::TcpServer(std::string(argv[1]), std::stoi(argv[2]));
        server.setup_handler(&test);
        server.startServer();
    }
    else{
        perror("http_tcp_linux <ip address> <port>");
        exit(1);
    }

}