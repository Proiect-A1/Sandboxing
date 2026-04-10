#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include "headers/json.hpp"
#include <fcntl.h>

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


void send(int fd , string data)
{
    int length = data.size(); 
    write(fd , &length , sizeof(length));
    write(fd , data.c_str() , length);
}

void *read_thread(void *argv)
{
    char *ip;
    short port;
    read_args((char **) argv , ip , port);
    sockaddr_in socket_address = prepare_ip(ip , port);
    int sockfd = socket(AF_INET , SOCK_STREAM , 0); if(sockfd == -1) handle_error(1 , "socket()");
    if(connect(sockfd , (sockaddr *) &socket_address , sizeof(socket_address)) == -1) handle_error(1 , "connect()");

    // int length = 1e6; write(sockfd , &length , sizeof(length));

    // for(int i = 1 ; i <= 1e6 ; i++)
    // {
    //     write(sockfd , "a" , 1);
    //     if(i == 1e4)
    //         return 0;
    // }

    // while(1)
    // {
    //     char ch;
    //     string filename;
    //     cin >> filename;

    //     cerr << "[client] received path:" << filename << '\n'; fflush(stderr);

    //     if(filename.find_last_of(".zip") == filename.size() - 1)
    //     {
    //         int fd = open(filename.c_str() , O_RDONLY);
    //         int length = lseek(fd , 0 , SEEK_END);
    //         lseek(fd , 0 , SEEK_SET);
    //         write(sockfd , &length , sizeof(length));

    //         for(int i = 0 ; i < length ; i++)
    //         {
    //             char ch; read(fd , &ch , sizeof(ch));
    //             write(sockfd , &ch , sizeof(ch));
    //         }

    //         cerr << "[client] sent zip\n"; fflush(stderr);
    //     }
    //     else 
    //     {
    //         json j;
    //         ifstream f(filename);
    //         f >> j;

    //         send(sockfd , j.dump());
    //         cerr << "[client] sent file: " << j.dump() << '\n'; fflush(stderr);

    //         f.close();
    //     }
    // }

    return nullptr;
}

int main(int argc , char *argv[])
{
    if(argc != 3) handle_error(1 , "Provide IP PORT");

    char *ip;
    short port;
    read_args(argv , ip , port);
    sockaddr_in socket_address = prepare_ip(ip , port);
    int sockfd = socket(AF_INET , SOCK_STREAM , 0); if(sockfd == -1) handle_error(1 , "socket()");
    if(connect(sockfd , (sockaddr *) &socket_address , sizeof(socket_address)) == -1) handle_error(1 , "connect()");

    while(1)
    {
        char ch;
        string filename;
        cin >> filename;

        cerr << "[client] received path:" << filename << '\n'; fflush(stderr);

        if(filename.find_last_of(".zip") == filename.size() - 1)
        {
            int fd = open(filename.c_str() , O_RDONLY);
            int length = lseek(fd , 0 , SEEK_END);
            lseek(fd , 0 , SEEK_SET);
            write(sockfd , &length , sizeof(length));

            for(int i = 0 ; i < length ; i++)
            {
                char ch; read(fd , &ch , sizeof(ch));
                write(sockfd , &ch , sizeof(ch));
            }

            cerr << "[client] sent zip\n"; fflush(stderr);
        }
        else 
        {
            json j;
            ifstream f(filename);
            f >> j;

            send(sockfd , j.dump());
            cerr << "[client] sent file: " << j.dump() << '\n'; fflush(stderr);

            f.close();
        }
    }

    return 0;
}

