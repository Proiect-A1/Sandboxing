#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <Server/json.hpp>
#include <Server/header_helper.hpp>
#include <fcntl.h>
using namespace std;


int sockfd;

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

string find_extension(string filename)
{
    for(int i = filename.size() - 1 ; i >= 0 ; i--)
        if(filename[i] == '.')
            return filename.substr(i , string::npos);

    return string("");
}

void send_file(string filename)
{
    LOG_INFO(std::string("received path: ") + filename);

    if(find_extension(filename) != ".json")
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

        LOG_INFO("sent file");
    }
    else 
    {
        json j;
        ifstream f(filename);
        f >> j;

        send(sockfd , j.dump());
        LOG_INFO(std::string("sent file: ") + j.dump());

        f.close();
    }
}

int read_consistent(int fd , void *data , int len)
{
    int total_read = 0;

    while(len)
    {
        int read_len = read(fd , data , len); if(read_len == -1) return -1;
        total_read += read_len;
        data = (void *)((unsigned long long)(data) + read_len);
        len -= read_len;
    }

    return total_read;
}

void test_submission_protocol()
{
    send_file("testing_data/evaluate_request.json");
    send_file("testing_data/submission_swapsort.cpp");
    
    int length; read_consistent(sockfd , &length , sizeof(length));
    string json;
    
    for(int i = 1 ; i <= length ; i++)
    {
        char ch; read_consistent(sockfd , &ch , sizeof(ch));
        json += ch;
    }

    cerr << json; fflush(stderr);
    send_file("testing_data/send_problem_request.json");
    send_file("testing_data/swapsort.1");

    while(1)
    {
        int length; read_consistent(sockfd , &length , 4);

        for(int i = 0 ; i  < length; i++)
        {
            char ch; read_consistent(sockfd , &ch , sizeof(ch));
            cout << ch; fflush(stdout);
        }

        cout << endl;
    }
}

int main(int argc , char *argv[])
{
    if(argc != 3) handle_error(1 , "Provide IP PORT");

    char *ip;
    short port;
    read_args(argv , ip , port);
    sockaddr_in socket_address = prepare_ip(ip , port);
    sockfd = socket(AF_INET , SOCK_STREAM , 0); if(sockfd == -1) handle_error(1 , "socket()");
    if(connect(sockfd , (sockaddr *) &socket_address , sizeof(socket_address)) == -1) handle_error(1 , "connect()");

    test_submission_protocol();
    while(1);
    return 0;
}

