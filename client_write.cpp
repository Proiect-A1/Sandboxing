#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include "headers/json.hpp"
#include <fcntl.h>
#include "headers/IO.hpp"

#define handle_error(ret_code , ...) { fprintf(stderr , __VA_ARGS__); exit(ret_code); }
using namespace std;
using json = nlohmann::json;


void read_args(char **argv , char *&ip , short &port)
{
    ip = (char *) malloc(strlen(argv[1]) + 1); memcpy(ip , argv[1] , strlen(argv[1]) + 1);
    sscanf(argv[2] , "%hd" , &port);
}

sockaddr_in prepare_ip(const char *ip , short port)
{
    in_addr addr;
    inet_aton(ip , &addr);

    sockaddr_in socket_address;
    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(port);
    socket_address.sin_addr = addr;
    return socket_address;
} 


int main(int argc , char *argv[])
{
    if(argc != 3) handle_error(1 , "Provide IP PORT");

    char *ip;
    short port;
    read_args(argv , ip , port);
    sockaddr_in socket_address = prepare_ip(ip , port);
    int sockfd = socket(AF_INET , SOCK_STREAM , 0); if(sockfd == -1) handle_error(1 , "socket()");
    if(bind(sockfd , (sockaddr *) &socket_address , sizeof(socket_address)) == -1) handle_error(1 , "bind()");
    if(listen(sockfd , 5) == -1) handle_error(1 , "listen()")
 
    sockaddr_in client_address;
    socklen_t len_client_address = sizeof(client_address);
    int fd;
    if((fd = accept(sockfd , (sockaddr *) &client_address , &len_client_address)) == -1) handle_error(1 , "accept()");

    char *msg;

    while(msg = IOhelper::recv(fd))
    {
        cout << msg << '\n'; fflush(stdout);
    }
    
    
    return 0;
}

