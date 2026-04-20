#include <Server/IO.hpp>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <Server/exceptions.hpp>
#include <Singletoni/submission_manager.h>
#include <Singletoni/task_queue.h>
#include <Singletoni/user_queue.h>
#include <Tasks/evaluator_task.h>

using namespace std;

const char *bd_ip = "10.8.0.9";
const short bd_port = 6767;

IO::IO()
{
    current_pos = 0 , length = 0;
}

void IO::execute(string command , json j , int fd)
{
    if(command == "evaluate") evaluate_request(j , fd);
    if(command == "sendProblem") send_problem_request(j , fd);
}

int IO::read_consistent_w_buffer(int fd , void *data , int len)
{
    try 
    {
        int total_read = 0;

        while(len)
        {
            char byte = get_char_fd(fd); 
            *((char *) data) = byte;
            data = (void *)((unsigned long long)(data) + 1);
            len--;
            total_read++;
        }

        return total_read;
    }
    catch(exception &e)
    {
        throw;
    }
   
}

int IO::read_consistent(int fd , void *data , int len)
{
    int total_read = 0;

    while(len)
    {
        int read_len = read(fd , data , len); if(read_len == -1) return -1;
        if(read_len == 0) throw unfinished_request_exception();
        total_read += read_len;
        data = (void *)((unsigned long long)(data) + read_len);
        len -= read_len;
    }

    return total_read;
}

char IO::get_char_fd(int fd)
{
    if(current_pos == length)
    {
        length = read(fd , buff , BUFF_SIZE);
        if(length == -1) handle_error(1 , "get_char_fd()");
        if(length == 0) throw unfinished_request_exception();
        current_pos = 0;
    }

    return buff[current_pos++];
}

sockaddr_in IO::prepare_ip(const char *ip , short port)
{
    in_addr addr;
    inet_aton(ip , &addr);

    sockaddr_in socket_address;
    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(port);
    socket_address.sin_addr = addr;
    return socket_address;
} 

int IO::create_socket()
{
    sockaddr_in bd_address = prepare_ip(bd_ip , bd_port);
    int sockfd = socket(AF_INET , SOCK_STREAM  , 0); if(sockfd == -1) handle_error(1 , "socket() create_socket()")
    if(connect(sockfd , (sockaddr *) &bd_address , sizeof(bd_address)) == -1) handle_error(1 , "connect() create_socket()");
    fprintf(stderr , "[thread] connected to %s:%hd\n" , inet_ntoa(bd_address.sin_addr) , ntohs(bd_address.sin_port)); fflush(stderr);
    return sockfd;
}

void IO::send(const char *msg , int fd)
{
    int length = strlen(msg);
    if(write(fd , &length , sizeof(length)) != sizeof(length)) handle_error(1 , "write() send()");
    if(write(fd , msg , length) != length) handle_error(1 , "write() send()")
}

string IO::recv(int fd) 
{
    try 
    {
        int length; if(read_consistent(fd , &length , sizeof(length)) != sizeof(length)) handle_error(1 , "read_consistent()");
        string payload;
        payload.resize(length + 1);

        if(read_consistent(fd , (void *) payload.c_str() , length) != length) handle_error(1 , "read_consistent()");
        payload[length] = '\0';
        return payload;
    }
    catch(exception &e)
    {
        throw;
    }
    
}

void IO::done_test_request(string submissionId , int testId , int verdict , string message , float score , float maxScore , float scorePercent , long long memory , long long time)
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
    IO::send(request.dump().c_str() , sockfd);
    close(sockfd);
}

void IO::done_subtask_request(string submissionId , int subtaskId , float score , float maxScore , float scorePercent , long long maxMemory , long long maxTime)
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

void IO::done_submission_request(string submissionId , float score , float maxScore , float scorePercent , long long maxMemory , long long maxTime)
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

void IO::upload_tests_request(string problemId , int revId , string archiveType , vector < vector < int > > groups , int archive_fd)
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

void IO::pull_problem_request(string problemId)
{
    int sockfd = create_socket();
    json request;
    request["request"] = "pullProblem";
    request["problemId"] = problemId;
    send(request.dump().c_str() , sockfd);
    close(sockfd);
}

void IO::evaluate_request(json request , int fd)
{
    try 
    {
        char path[PATH_MAX];
        sprintf(path , "%s/submissions/%s" , getenv("SANDBOX_PATH") , request["submissionId"].get < string > ().c_str());
        if(mkdir(path , 0770) == -1) handle_error(1 , "mkdir() evaluate_request()");
        sprintf(path , "%s/submissions/%s/main.%s" , getenv("SANDBOX_PATH") , request["submissionId"].get < string > ().c_str() , request["language"].get < string > ().c_str());

        int submission_fd = open(path , O_CREAT | O_TRUNC | O_RDWR , 0660); if(submission_fd == -1) handle_error(1 , "open() evaluate_request()");
        int length; if(read_consistent_w_buffer(fd , &length , sizeof(length)) != sizeof(length)) handle_error(1 , "read_consistent() evaluate_request()");

        for(int i = 1 ; i <= length ; i++)
        {
            char byte = get_char_fd(fd);
            if(write(submission_fd , &byte , sizeof(byte)) != sizeof(byte)) handle_error(1 , "write() send_problem_request()");
        }

        close(submission_fd);

        //insert in coada
        //unsafe
        //problema trebuie sa exista in memorie in momentul asta

        submission_manager& sm = submission_manager::get_instance();

        string submission_id = request["submissionId"].get < string > ();
        int rev_id = request["revId"].get < int > ();
        string problem_id = request["problemId"].get < string > ();

        sm.insert(submission_id, language_enum::CPP, problem_id , rev_id , 1);

        submission_data submission = sm.get_submission(submission_id);

        evaluator_task* eva = new evaluator_task(submission_id , problem_id , rev_id);

        task_queue::get_instance().push(eva);
    }
    catch(exception &e)
    {
        throw;
    } 
}

void IO::send_problem_request(json request , int fd)
{
    try
    {
        char path[PATH_MAX];
        sprintf(path , "%s/tmp/%s.%d" , getenv("SANDBOX_PATH") , request["problemId"].get < string > ().c_str() , request["revId"].get < int > ());
        
        int problem_fd = open(path , O_CREAT | O_TRUNC | O_RDWR , 0600);
        int length; if(read_consistent_w_buffer(fd , &length , sizeof(length)) == -1) handle_error(1 , "read_consistent() evaluate_request()");
        //need to finish

        for(int i = 1 ; i <= length ; i++)
        {
            char byte = get_char_fd(fd);
            if(write(problem_fd , &byte , sizeof(byte)) != sizeof(byte)) handle_error(1 , "write() send_problem_request()"); 
        }

        close(problem_fd);
    }
    catch(exception &e)
    {
        throw;
    }
    
}