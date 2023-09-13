#include "../include/http_tcp_linux.h"

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>


#include <iostream>
#include <sstream>

namespace http {
    TcpServer::TcpServer(std::string ip_address, int port):ip_address(ip_address), port(port){
        socketAddress.sin_family = AF_INET;
        socketAddress.sin_port = htons(port);
        socketAddress.sin_addr.s_addr = inet_addr(ip_address.c_str());
        memset(socketAddress.sin_zero, 0, 8);  
        server_socketAddress_len = sizeof(socketAddress);
    }
    TcpServer::~TcpServer(){
        stopServer();
    }

    std::string TcpServer::create_response(std::string message){
        std::ostringstream ss;
        ss << TcpServer::header_text << std::to_string(message.size()) << "\n\n" << message;
        return ss.str();
    }


    void TcpServer::getServerAddr(){
        struct socketaddr_in sin;
        socklen_t len = sizeof(sin);
        if(getsockname(server_socket_fd, (sockaddr *)&sin, &server_socketAddress_len) < 0){
            exitWithError("Could not obtain server addr");
        }
        std::cout<< "Server Addr: " << ip_address << std::endl;
        std::cout<< "Port #: " << std::to_string(ntohs(sin.sin_port)) << std::endl;
        port = sin.sin_port;
    }


    int TcpServer::startServer(){
        // assigns fd 
        server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket_fd < 0){
            TcpServer::exitWithError("Cannot Open Socket");
            return 1;
        }
        if (bind(server_socket_fd,(sockaddr *)&socketAddress, server_socketAddress_len) < 0){
            exitWithError("Cannot connect socket to address");
            return 1;
        }
        getServerAddr(); 
        // begin listening for connection attempts        
        if(listen(server_socket_fd, 3) < 0){
            exitWithError("Cannot innitialize listening");
            return 1;
        }
        int bytesRecieved;
        int bytesSent;
        while(1){
            std::cout << "Waiting for connection...\n";
            server_new_socket_fd = accept(server_socket_fd,(sockaddr *)&socketAddress, &server_socketAddress_len);
            if(server_new_socket_fd < 0){
                std::cout<<"socket fail\n";
                continue;
            }
            char buffer[BUFFER_SIZE] = {0};
            bytesRecieved = recv(server_new_socket_fd, buffer, BUFFER_SIZE, 0);
            if(bytesRecieved < 0){
                std::cout<<"recieve fail: " << bytesRecieved << "\n";
                fprintf(stderr, "recv: %s (%d)\n", strerror(errno), errno);
                continue;
            }

            std::cout<< "Connected Accepted ...\n";
            std::cout<< "Client Message: " << buffer << std::endl;

            (*function_prt)(buffer, this);
        }   
        return 0;   
    }

    void TcpServer::send_response(std::string response){
        long totalBytesSent = 0;
        int bytesSent;
        while (totalBytesSent < response.size()){
            bytesSent = send(server_new_socket_fd, response.c_str(), response.size(), 0);
            if (bytesSent < 0){
                break;
            }
            std::cout<<"Sending response...\n";
            totalBytesSent += bytesSent;
        }
        if (totalBytesSent == response.size()){
            std::cout<< "Message Sent\n\n";
        }
        else{
            std::cout<< "Message could not be sent\n\n";
        }
        close(server_new_socket_fd);
    }   

    void TcpServer::setup_handler(void (*new_function_prt)(char incoming_message[BUFFER_SIZE], TcpServer * server)){
        function_prt = new_function_prt;
    }



    void TcpServer::stopServer(){ // close all open socket fds
        close(server_socket_fd);
        close(server_new_socket_fd);
        exit(0);
    }

    void TcpServer::exitWithError(const std::string err_message){
        std::cout<< "ERROR: " << err_message;
        exit(1);
    }
} // namescape http 