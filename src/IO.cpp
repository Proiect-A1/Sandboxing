#include "../headers/IO.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

const char *bd_ip = "10.8.0.9";
const short bd_port = 6000;

namespace IOhelper 
{
    int current_pos = 0 , length = 0;
    char buff[BUFF_SIZE];

    int read_consistent(int fd , void *data , int len)
    {
        int total_read = 0;

        while(len)
        {
            int read_len = read(fd , data , len); if(read_len == -1) return -1;
            if(read_len == 0) return total_read;
            total_read += read_len;
            data = (void *)((unsigned long long)(data) + read_len);
            len -= read_len;
        }

        return total_read;
    }

    char get_char_fd(int fd)
    {
        if(current_pos == length)
        {
            length = read(fd , buff , BUFF_SIZE);
            if(length == -1) handle_error(1 , "get_char_fd()");
            if(length == 0) handle_error(1 , "get_char_fd()");
            current_pos = 0;
        }

        return buff[current_pos++];
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

    int create_socket()
    {
        sockaddr_in bd_address = prepare_ip(bd_ip , bd_port);
        int sockfd = socket(AF_INET , SOCK_STREAM  , 0); if(sockfd == -1) handle_error(1 , "socket() create_socket()")
        if(connect(sockfd , (sockaddr *) &bd_address , sizeof(bd_address)) == -1) handle_error(1 , "connect() create_socket()");
        return sockfd;
    }

    void send(const char *msg , int fd)
    {
        int length = strlen(msg);
        if(write(fd , &length , sizeof(length)) != sizeof(length)) handle_error(1 , "write() send()");
        if(write(fd , msg , length) != length) handle_error(1 , "write() send()")
    }

    char *recv(int fd)
    {
        int length; if(read_consistent(fd , &length , sizeof(length)) != length) handle_error(1 , "read_consistent()");
        char *payload = (char *) malloc(length + 1);
        read_consistent(fd , payload , length);
        payload[length] = '\0';
        return payload;

    }

    void done_test_request(string submissionId , int testId , int verdict , string message , float score , float maxScore , float scorePercent , long long memory , long long time)
    {   
        int sockfd = create_socket();
        json request;
        request["submissionId"] = submissionId;
        request["testId"] = testId;
        request["verdict"] = verdict;
        request["message"] = message;
        request["score"] = score;
        request["maxScore"] = maxScore;
        request["score%"] = scorePercent;
        request["memory"] = memory;
        request["time"] = time;
        send(request.dump().c_str() , sockfd);
        close(sockfd);
    }

    // void done_subtask_request(string submissionId , int subtaskId , float score , float maScore , float scorePercent , long long max_memory , long long max_time);
    // void done_submission_request(string submissionId , float score , float maxScore , float scorePercent , long long max_memory , long long max_time);
    // void upload_tests_request(string problemId , int revId , string archiveType , vector < vector < int > > groups , int archive_fd);
    // void pull_problem_request(string problemId);

    // void evaluate_request(json request , int fd);
    // void send_problem_request(json request , int fd);
}