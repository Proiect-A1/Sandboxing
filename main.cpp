#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include "headers/json.hpp"
#include "headers/IO.hpp"
#include <fcntl.h>

#define EVENTS_BUFF_SIZE 4096

#define handle_error(ret_code , ...) { fprintf(stderr , __VA_ARGS__); exit(ret_code); }

using namespace std;
using json = nlohmann::json;

char *ip;
short port;
short num_of_threads;
int sockfd;
int epollfd;

void read_args(int argc , char *argv[])
{
    ip = (char *) malloc(strlen(argv[1]) + 1); memcpy(ip , argv[1] , strlen(argv[1]) + 1);
    sscanf(argv[2] , "%hd" , &port);
    sscanf(argv[3] , "%hd" , &num_of_threads);
}

void add_fd(int fd , int events)
{
    epoll_event event;
    event.events = events;
    epoll_data data; data.fd = fd;
    event.data = data;
    if(epoll_ctl(epollfd , EPOLL_CTL_ADD , fd , &event) == -1) handle_error(1 , "epoll_ctl()");
}

void rem_fd(int fd)
{
    if(epoll_ctl(epollfd , EPOLL_CTL_DEL , fd , nullptr) == -1) handle_error(1 , "epoll_ctl()");
    sockaddr_in client_address;
    socklen_t len_client_address = sizeof(client_address);
    if(getpeername(fd , (sockaddr *) &client_address , &len_client_address) == -1) handle_error(1 , "getperrname()");
    fprintf(stderr , "[server] connection closed by %s:%hu\n" , inet_ntoa(client_address.sin_addr) , ntohs(client_address.sin_port)); fflush(stderr);
}

void set_socket()
{
    sockaddr_in socket_address = IOhelper::prepare_ip(ip , port);
    sockfd = socket(AF_INET , SOCK_STREAM , 0); if(sockfd == -1) handle_error(1 , "socket()");
    int on = 1; if(setsockopt(sockfd , SOL_SOCKET , SO_REUSEPORT , (const char *) &on , sizeof(on)) == -1) handle_error(1 , "setsockopt()")
    if(bind(sockfd , (sockaddr *) &socket_address , sizeof(socket_address)) == -1) handle_error(1 , "bind()");
    if(listen(sockfd , 5) == -1) handle_error(1 , "listen()")
    cerr << "[server] socket set\n"; fflush(stderr);
}

void create_epoll()
{
    epollfd = epoll_create1(0); if(epollfd == -1) handle_error(1 , "epoll_create1()");
}

int accept_new_connection()
{
    sockaddr_in client_address;
    socklen_t len_client_address = sizeof(client_address);
    int fd;
    if((fd = accept(sockfd , (sockaddr *) &client_address , &len_client_address)) == -1) handle_error(1 , "accept()");
    fprintf(stderr , "[server] connection received from %s:%hu\n" , inet_ntoa(client_address.sin_addr) , ntohs(client_address.sin_port)); fflush(stderr);
    return fd;
}

void * main_thread(void *arg)
{
    int thread_count = *((int *) arg);
    //campeaza coada
    return nullptr;
}

void create_threads()
{
    pthread_t *thread_ids = (pthread_t *) malloc(num_of_threads * sizeof(pthread_t));
    int *thread_counts = (int *) malloc(num_of_threads * sizeof(int));

    for(int i = 0 ; i < num_of_threads ; i++)
    {
        thread_counts[i] = i;
        if(pthread_create(&thread_ids[i] , nullptr , main_thread , (void *) &thread_counts[i]) != 0) handle_error(1 , "pthread_create()");
        if(pthread_detach(thread_ids[i]) != 0) handle_error(1 , "pthread_detach()");
    }
}

void receive_request(int client_fd)
{
    int len_read;
    int length; if((len_read = IOhelper::read_consistent(client_fd , &length , sizeof(length))) == -1) handle_error(1 , "read_consistent()");
    if(len_read == 0) {rem_fd(client_fd); return;}

    string request_string;

    for(int i = 0 ; i < length ; i++)
    {
        char ch = IOhelper::get_char_fd(client_fd);
        request_string += ch;
    }

    json j = json::parse(request_string);
    cerr << j.dump() << '\n'; fflush(stderr);

    if(j.contains("zip"))
    {
        int len_read;
        int length; if((len_read = IOhelper::read_consistent(client_fd , &length , sizeof(length))) == -1) handle_error(1 , "read_consistent()");
        if(len_read == 0) {rem_fd(client_fd); return;}

        string filename = j["zip"].get < string > ();
        int fd = open(filename.c_str() , O_WRONLY | O_CREAT | O_TRUNC , 0600);

        for(int i = 0 ; i < length ; i++)
        {
            char byte = IOhelper::get_char_fd(client_fd);
            write(fd , &byte , sizeof(byte));
        }

        close(fd);
        cerr << "[server] received zip\n"; fflush(stderr);
    }
    
}

int main(int argc , char *argv[])
{
    if(argc != 4) handle_error(1 , "Provide IP PORT number of threads");

    IOhelper::done_test_request("hello" , 1 , 10 , "abcd" , 10.5 , 100.4 , 32 , 1000 , 10000);

    return 0;
    
    read_args(argc , argv);
    set_socket();
    create_epoll();
    add_fd(sockfd , EPOLLIN);
    create_threads();

    cerr << "[server] epoll set\n"; fflush(stderr);
    epoll_event ev[EVENTS_BUFF_SIZE];
    int num_events;

    while((num_events = epoll_wait(epollfd , ev , EVENTS_BUFF_SIZE , -1)) > 0)
    {
        for(int i = 0 ; i < num_events ; i++)
        {
            int events_mask = ev[i].events;
            int fd = ev[i].data.fd;

            if(events_mask & EPOLLIN)
            {
                if(fd == sockfd)
                {
                    int fd_client = accept_new_connection();
                    add_fd(fd_client , EPOLLIN | EPOLLET);
                }   
                else 
                {
                    cerr << "[server] request received\n"; fflush(stderr);
                    receive_request(fd);
                }
            }
        }
    }
    
    handle_error(1 , "epoll_wait()");  
    close(epollfd);

    return 0;
}