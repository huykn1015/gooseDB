#ifndef GOOSEDB
#define GOOOSEDB

#include "/Users/huykn/Desktop/gooseDB/HTTP_/include/http_tcp_linux.h"
#include "/Users/huykn/Desktop/gooseDB/kvs/include/kvs.h"

namespace gooseDB{


    class GooseServer{
    public:
        GooseServer();
        ~GooseServer();
        int startServer(std::string ip, int port = 0);

    private:
        void message_handler(char incoming_message[http::BUFFER_SIZE], http::TcpServer * server);
        int processRequest(std::string request, std::string &return_message);
        std::string createRequest(std::string name);
        std::string getRequest(std::string token, std::string key);
        std::string addRequest(std::string token, std::string key, std::string value);
        std::string updateRequest(std::string token, std::string key, std::string value);
        std::string deleteRequest(std::string token, std::string key);


    };






}





#endif


// CREATE name = XXXXX   -> return plaintext token + key
// GET token=XXXXX key = XXXXXX -> return plaintext value or error message
// ADD token= XXXXX key = XXXXX -> return plaintext confirmation or error message
// UPDATE token = XXXXX key = XXXXX -> return plaintext confirmation or error message
// DELETE token = XXXXX key = XXXXX <- if key == edit token delete db -> return plaintext confirmation or error message


// handler -> process -> process calls 1/5 FCNS -> return strings
// handler calls send result of process