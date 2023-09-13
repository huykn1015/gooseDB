#ifndef INCLUDED_HTTP_TCP_LINUX
#define INCLUDED_HTTP_TCP_LINUX

#include <sys/types.h>
#include <sys/socket.h>

#include <string>



namespace http {
    enum{BUFFER_SIZE = 3072};

    struct in_addr_{
        uint32_t s_addr; 
    };

    struct socketaddr_in {
        short             sin_family;
        unsigned short      sin_port;
        struct in_addr_      sin_addr;
        char             sin_zero[8];
    };

    struct addrinfo{
        int ai_flags;
        int ai_family;
        int ai_socktype;
        int ai_protocol;
        size_t addr_len;
        struct sockaddr * ai_addr;
        char * ai_canonname;

        struct addrinfo * ai_next;    
    };
    class TcpServer {
    public:
        const int MAX_SOCKET_CONNECTIONS = 5;
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
        std::string test_message = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length:12 \n\nHello World!"; //100\n\n<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
        std::string header_text = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length:";
        TcpServer(std::string ip_address, int port = 0);
        ~TcpServer();
        std::string create_response(std::string message);
        void send_response(std::string response);
        int  startServer(); // start server and begin listening for connections
        void getServerAddr();
        void setup_handler(void (*new_function_prt)(char incoming_message[BUFFER_SIZE], TcpServer * server));
    private:
        std::string ip_address; // ip address of the server socket
        int port; //port of the server socket
        int server_socket_fd; // socket listening for new clients
        int server_new_socket_fd; // socket for communicating with clients
        void (*function_prt)(char incoming_message[BUFFER_SIZE], TcpServer * server); // function pointer to message handler
        struct socketaddr_in socketAddress; // information about the socket
        socklen_t  server_socketAddress_len;
        void stopServer(); // stop server and close all sockets
        void exitWithError(const std::string err_message);
    };
};
#endif