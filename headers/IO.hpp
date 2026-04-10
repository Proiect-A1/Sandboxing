#ifndef IO_H
#define IO_H

#include <bits/stdc++.h>
#include "header_helper.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#define BUFF_SIZE 4096

using namespace std;

class IO 
{
    private:
        int current_pos , length;
        char buff[BUFF_SIZE];

    public:
        IO();
        static int read_consistent(int fd , void *data , int len);
        int read_consistent_w_buffer(int fd , void *data , int len);
        
        char get_char_fd(int fd);
        static void send(const char *msg , int fd);
        static string recv(int fd);

        static sockaddr_in prepare_ip(const char *ip , short port);
        static int create_socket();

        static void done_test_request(string submissionId , int testId , int verdict , string message , float score , float maxScore , float scorePercent , long long memory , long long time);
        static void done_subtask_request(string submissionId , int subtaskId , float score , float maScore , float scorePercent , long long max_memory , long long max_time);
        static void done_submission_request(string submissionId , float score , float maxScore , float scorePercent , long long max_memory , long long max_time);
        void upload_tests_request(string problemId , int revId , string archiveType , vector < vector < int > > groups , int archive_fd);
        static void pull_problem_request(string problemId);

        void evaluate_request(json request , int fd);
        void send_problem_request(json request , int fd);
};

#endif 