#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include "headers/json.hpp"

#define handle_error(ret_code , ...) { fprintf(stderr , __VA_ARGS__); exit(ret_code); }
using namespace std;
using json = nlohmann::json;


char *ip;
short port;
int sockfd;

void read_args(int argc , char *argv[])
{
    ip = (char *) malloc(strlen(argv[1]) + 1); memcpy(ip , argv[1] , strlen(argv[1]) + 1);
    sscanf(argv[2] , "%hd" , &port);
}

sockaddr_in prepare_ip()
{
    in_addr addr;
    inet_aton(ip , &addr);

    sockaddr_in socket_address;
    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(port);
    socket_address.sin_addr = addr;
    return socket_address;
} 


void send(int fd , string data)
{
    int length = data.size(); 
    write(fd , &length , sizeof(length));
    write(fd , data.c_str() , length);
}

int main(int argc , char *argv[])
{
    if(argc != 3) handle_error(1 , "Provide IP PORT");

    read_args(argc , argv);
    sockaddr_in socket_address = prepare_ip();
    sockfd = socket(AF_INET , SOCK_STREAM , 0); if(sockfd == -1) handle_error(1 , "socket()");
    if(connect(sockfd , (sockaddr *) &socket_address , sizeof(socket_address)) == -1) handle_error(1 , "connect()");

    while(1)
    {
        char ch;
        string filename;
        cin >> filename;

        json j;
        ifstream f(filename);
        f >> j;

        send(sockfd , j.dump());

        f.close();
    }

    return 0;
}

