#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include "headers/json.hpp"
#include "headers/IO.hpp"

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
}
void set_socket()
{
    sockaddr_in socket_address = prepare_ip();
    sockfd = socket(AF_INET , SOCK_STREAM , 0); if(sockfd == -1) handle_error(1 , "socket()");
    int on = 1; if(setsockopt(sockfd , SOL_SOCKET , SO_REUSEPORT , (const char *) &on , sizeof(on)) == -1) handle_error(1 , "setsockopt()")
    if(bind(sockfd , (sockaddr *) &socket_address , sizeof(socket_address)) == -1) handle_error(1 , "bind()");
    if(listen(sockfd , 5) == -1) handle_error(1 , "listen()")
}

void create_epoll()
{
    epollfd = epoll_create1(0); if(epollfd == -1) handle_error(1 , "epoll_create1()");
}

int accept_new_connection()
{
    sockaddr_in client_address;
    socklen_t len_client_address;
    int fd;
    if((fd = accept(sockfd , (sockaddr *) &client_address , &len_client_address)) == -1) handle_error(1 , "accept()");
    return fd;
}

int read_consistent(int fd , void *data , int len)
{
    while(len)
    {
        int read_len = read(fd , data , len); if(read_len == -1) return -1;
        data = (void *)((long long)(data) + read_len);
        len -= read_len;
    }

    return len;
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
    int length; if((len_read = read(client_fd , &length , sizeof(length))) == -1) handle_error(1 , "read()");
    if(len_read == 0) {rem_fd(client_fd); return;}

    string request_string;

    for(int i = 0 ; i < length ; i++)
    {
        char ch = IOhelper::get_char_fd(client_fd);
        request_string += ch;
    }

    json j = json::parse(request_string);
    cerr << j.dump(); fflush(stderr);
}

int main(int argc , char *argv[])
{
    if(argc != 4) handle_error(1 , "Provide IP PORT number of threads");

    read_args(argc , argv);
    set_socket();
    create_epoll();
    add_fd(sockfd , EPOLLIN);
    create_threads();

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
                    cerr << "Connection received\n"; fflush(stderr);
                    add_fd(fd_client , EPOLLIN | EPOLLET);
                }   
                else 
                {
                    receive_request(fd);
                }
            }
        }
    }
    
    handle_error(1 , "epoll_wait()");  
    close(epollfd);

    return 0;
}