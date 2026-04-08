#include "../headers/IO.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

using namespace std;

const char *bd_ip = "127.0.0.1";
const short bd_port = 6001;

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
        int length; if(read_consistent(fd , &length , sizeof(length)) != sizeof(length)) handle_error(1 , "read_consistent()");
        char *payload = (char *) malloc(length + 1);
        if(read_consistent(fd , payload , length) != length) handle_error(1 , "read_consistent()");
        payload[length] = '\0';
        return payload;

    }

    void done_test_request(string submissionId , int testId , int verdict , string message , float score , float maxScore , float scorePercent , long long memory , long long time)
    {   
        int sockfd = create_socket();
        json request;
        request["request"] = "doneTest";
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

    void done_subtask_request(string submissionId , int subtaskId , float score , float maxScore , float scorePercent , long long maxMemory , long long maxTime)
    {
        int sockfd = create_socket();
        json request;
        request["request"] = "doneSubtask";
        request["submissionId"] = submissionId;
        request["subtaskId"] = subtaskId;
        request["score"] = score;
        request["maxScore"] = maxScore;
        request["score%"] = scorePercent;
        request["maxMemory"] = maxMemory;
        request["maxTime"] = maxTime; 
        send(request.dump().c_str() , sockfd);
        close(sockfd);
    }

    void done_submission_request(string submissionId , float score , float maxScore , float scorePercent , long long maxMemory , long long maxTime)
    {
        int sockfd = create_socket();
        json request;
        request["request"] = "doneSubmission"; 
        request["submissionId"] = submissionId;
        request["score"] = score;
        request["maxScore"] = maxScore;
        request["score%"] = scorePercent;
        request["maxMemory"] = maxMemory;
        request["maxTime"] = maxTime; 
        send(request.dump().c_str() , sockfd);
        close(sockfd);
    }

    void upload_tests_request(string problemId , int revId , string archiveType , vector < vector < int > > groups , int archive_fd)
    {
        int sockfd = create_socket();
        json request;
        request["request"] = "uploadTests";
        request["problemId"] = problemId;
        request["revId"] = revId;
        request["archiveType"] = archiveType;
        request["groups"] = groups;
        send(request.dump().c_str() , sockfd);

        int archive_length = lseek(archive_fd , 0 , SEEK_END); if(archive_length == -1) handle_error(1 , "lseek() upload_tests_request()");
        if(lseek(archive_fd , 0 , SEEK_SET) == -1) handle_error(1 , "lseek() upload_tests_request()");;

        if(write(sockfd , &archive_length , sizeof(archive_length)) != sizeof(archive_length)) handle_error(1 , "write() upload_tests()");

        int length_read = 0;

        while((length_read = read(archive_fd , buff , BUFF_SIZE)) != 0)
        {
            if(length_read == -1) handle_error(1 , "read() upload_tests()");
            if(write(sockfd , buff , length_read) != length_read) handle_error(1 , "write() upload_tests");
        }

        close(sockfd);
    }

    void pull_problem_request(string problemId)
    {
        int sockfd = create_socket();
        json request;
        request["request"] = "pullProblem";
        request["problemId"] = problemId;
        send(request.dump().c_str() , sockfd);
        close(sockfd);
    }

    void evaluate_request(json request , int fd)
    {
        int submission_fd = open(request["submissionId"].get < string > ().c_str() , O_CREAT | O_TRUNC | O_RDWR , 0600);
        int length; if(read_consistent(fd , &length , sizeof(length)) == -1) handle_error(1 , "read_consistent() evaluate_request()");
        //need to finish

        while(length)
        {
            int length_read = read(fd , buff , BUFF_SIZE); if(length_read == -1) handle_error(1 , "read() evaluate_request()");
            if(write(submission_fd , buff , length_read) != length_read) handle_error(1 , "write() evalute_request()");
            length -= length_read;
        }

        close(submission_fd);
    }

    void send_problem_request(json request , int fd)
    {
        int problem_fd = open(request["problemId"].get < string > ().c_str() , O_CREAT | O_TRUNC | O_RDWR , 0600);
        int length; if(read_consistent(fd , &length , sizeof(length)) == -1) handle_error(1 , "read_consistent() evaluate_request()");
        //need to finish

        while(length)
        {
            int length_read = read(fd , buff , BUFF_SIZE); if(length_read == -1) handle_error(1 , "read() evaluate_request()");
            if(write(problem_fd , buff , length_read) != length_read) handle_error(1 , "write() evalute_request()");
            length -= length_read;
        }

        close(problem_fd);
    }
}