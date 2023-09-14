#include "/Users/huykn/Desktop/gooseDB/include/gooseDB.h"

namespace gooseDB {

    void message_handler(char incoming_message[http::BUFFER_SIZE], http::TcpServer * server){
        
    }
    

    int GooseServer::startServer(std::string ip_address, int port){
        http::TcpServer server = http::TcpServer(std::string(ip_address), port));
        server.setup_handler(&(message_handler));
        server.startServer();
    }




}