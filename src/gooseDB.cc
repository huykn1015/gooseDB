#include "/Users/huykn/Desktop/gooseDB/include/gooseDB.h"

#include <iostream>
#include <string>
#include <regex>
#include <sstream>

namespace gooseDB {

    const std::regex TOKEN_regex("\\stoken=([a-zA-Z0-9]*)\\s", std::regex_constants::icase);
    const std::regex KEY_regex("\\skey=([a-zA-Z0-9]*)\\s", std::regex_constants::icase);
    const std::regex VALUE_regex("\\svalue=([a-zA-Z0-9]*)\\s", std::regex_constants::icase);
    const std::regex NAME_regex("\\sname=([a-zA-Z][a-zA-Z0-9]*)\\s", std::regex_constants::icase);
    int GooseServer::startServer(std::string ip_address, int port){
        http::TcpServer server = http::TcpServer(std::string(ip_address), port);
        server.setup_handler(this);
        server.startServer();
        return 0;
    }

    void GooseServer::message_handler(char incoming_message[http::BUFFER_SIZE], http::TcpServer * server){
        std::string request(incoming_message);
        std::string return_message;
        if(processRequest(request, return_message)){
            std::cout<<"Error processing request" <<std::endl;
        }
        server->send_response(return_message);
    }

    int getDBSize(std::string request){
        const std::regex SIZE_regex("\\size=([0-9]*)\\s", std::regex_constants::icase);
        std::smatch matches;
        std::regex_search(request, matches, SIZE_regex);
        if(matches.empty()){
            return 4096;
        }
        return std::stoi(matches.str(1));
    }
    int GooseServer::processRequest(std::string request, std::string &return_message){
        std::smatch matches;
        std::regex_search(request, matches, TOKEN_regex);
        std::string token(matches.str(1));
        std::regex_search(request, matches, KEY_regex);
        std::string key(matches.str(1));
        std::regex_search(request, matches, VALUE_regex);
        std::string value(matches.str(1));
        std::regex_search(request, matches, NAME_regex);
        std::cout<<std::to_string(matches.empty())<<std::endl;
        std::string name(matches.str(1));
        if(request.find("GET") == 0){
            return_message = getRequest(name, token, key);
        }
        else if(request.find("SET") == 0){
            return_message = setRequest(name, token, key, value);
        }
        //else if(request.find("UPDATE") == 0){
        //    return_message = updateRequest(name, token, key, value);
        //}
        else if(request.find("DELETE") == 0){
            return_message = deleteRequest(name, token, key);
        }
        else if(request.find("CREATE") == 0){
            return_message = createRequest(name, getDBSize(request));
        }

        if (return_message == ""){
            return_message = "Could not process request";
            return 1;
        }
        return 0;
    }


    std::string GooseServer::createRequest(std::string name, int size){
        std::string read_token;
        std::string edit_token;
        std::cout<<"Create name: "<< name << std::endl;
        if(goose.create_kvs(name, edit_token, read_token, size)){
            return "DB already exists";
        }
        std::ostringstream ss;
        ss << "Successfully Created database \"" << name << "\"\n";
        ss << "Edit token: " << edit_token << std::endl;
        ss << "Read token: " << read_token << std::endl;
        return ss.str();
    }

    std::string GooseServer::getRequest(std::string name, std::string token, std::string key){
        goose.open_kvs(name, token);
        if(!goose.checkReadStatus()){
            goose.close_kvs();
            return "GET Request failed, Invalid read/edit token";
        }
        std::string value;
        if(goose.get_entry(key, value)){
            goose.close_kvs();
            return "GET Request failed, Key not found";
        }
        std::ostringstream ss;
        ss << "GET Request Successful, {" << key << ":" << value << "}" << std::endl;
        goose.close_kvs();
        return ss.str();
    }


    std::string GooseServer::setRequest(std::string name, std::string token, std::string key, std::string value){
        goose.open_kvs(name, token);
        if(!goose.checkEditStatus()){
            goose.close_kvs();
            return "SET Request failed, Invalid read/edit token";
        }
        if(goose.add_entry(key, value)){
            goose.close_kvs();
            return "SET Request failed";
        }
        std::ostringstream ss;
        ss << "SET Request Successful, Key: " << key << " has been set to value: " << value << std::endl;
        goose.close_kvs();
        return ss.str();
    }




    std::string GooseServer::deleteRequest(std::string name, std::string token, std::string key){
        goose.open_kvs(name, token);
        if(goose.checkEditToken(key)){
            goose.delete_db();
            return "DELETE Request Successful, Database \"" + name +"\" successfully deleted\n";
        }
        if(!goose.checkEditStatus()){
            goose.close_kvs();
            return "DELETE Request failed, Invalid read/edit token";
        }
        std::string value;
        if(goose.delete_entry(key)){
            goose.close_kvs();
            return "DELETE Request failed, Key does not exists";
        }
        std::ostringstream ss;
        ss << "DELETE Request Successful, Key: " << key << "has been deleted " << std::endl;
        goose.close_kvs();
        return ss.str();
    }

}