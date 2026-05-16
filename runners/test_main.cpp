#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <Server/json.hpp>
#include <Server/IO.hpp>
#include <fcntl.h>
#include <Server/tests.hpp>

#include <Tasks/evaluator_task.h>
#include <Tasks/preparator.h>
#include <Tasks/stdio_grader_task.h>
#include <iostream>
#include <Utilities/general_utilities.h>
#include <Singletoni/problem_manager.h>
#include <Singletoni/submission_manager.h>
#include <vector>
#include <Singletoni/user_queue.h>
#include <Singletoni/task_queue.h>
#include <Server/header_helper.hpp>
#include <pthread.h>

#define EVENTS_BUFF_SIZE 4096

using namespace std;

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
    LOG_INFO(std::string("Connection closed by ") + inet_ntoa(client_address.sin_addr) + ":" + std::to_string(ntohs(client_address.sin_port)));
}

void set_socket()
{
    sockaddr_in socket_address = IO::prepare_ip(ip , port);
    sockfd = socket(AF_INET , SOCK_STREAM , 0); if(sockfd == -1) handle_error(1 , "socket()");
    int on = 1; if(setsockopt(sockfd , SOL_SOCKET , SO_REUSEPORT , (const char *) &on , sizeof(on)) == -1) handle_error(1 , "setsockopt()")
    if(bind(sockfd , (sockaddr *) &socket_address , sizeof(socket_address)) == -1) handle_error(1 , "bind()");
    if(listen(sockfd , 5) == -1) handle_error(1 , "listen()")
    LOG_INFO(std::string("Listening socket on ") + inet_ntoa(socket_address.sin_addr) + ":" + std::to_string(ntohs(socket_address.sin_port)));
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
    LOG_INFO(std::string("Connection received from ") + inet_ntoa(client_address.sin_addr) + ":" + std::to_string(ntohs(client_address.sin_port)));
    return fd;
}

struct worker_thread_struct{
  pthread_t thread_id;
  task* current_task;
};

void *worker_thread(void *arg){
  struct worker_thread_struct *wts = (struct worker_thread_struct *) arg;
  pthread_t thread_id = wts->thread_id;
  task* current_task = wts->current_task;

  int user_id = user_queue::get_instance().pop();
  LOG_INFO_USER(user_id, "Starting task execution");
  const result_enum task_result = current_task->execute(thread_id, user_id);
  LOG_INFO_USER(user_id, "Finished task execution with result " + general_utilities::enum_to_string(task_result));
  user_queue::get_instance().push(user_id);

  delete current_task;
  delete wts;
  return nullptr;
}

void *main_thread(void *arg)
{
    int thread_count = *((int *) arg);
    //campeaza coada
    while (true){
      task* next_task = task_queue::get_instance().pop();

      worker_thread_struct* wts = new worker_thread_struct;
      wts->current_task = next_task;

      if(pthread_create(&(wts->thread_id), nullptr, worker_thread , (void *) wts) != 0) handle_error(1 , "pthread_create()");
      if(pthread_detach(wts->thread_id) != 0) handle_error(1 , "pthread_detach()");
    }

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

IO helper;

void receive_request(int client_fd)
{
    try 
    {
        int len_read;
        int length; if((len_read = helper.read_consistent_w_buffer(client_fd , &length , sizeof(length))) == -1) handle_error(1 , "read_consistent()");
        if(len_read == 0) {rem_fd(client_fd); return;}

        string request_string;

        for(int i = 0 ; i < length ; i++)
        {
            char ch = helper.get_char_fd(client_fd);
            request_string += ch;
        }
        
        LOG_DEBUG("payload received: " + request_string);
        
        json j = json::parse(request_string);
        LOG_DEBUG(std::string("Request string: ") + j.dump());
        
        if(!j.contains("request"))
        {
            LOG_ERROR("Invalid request received");
        }
        else 
        {
            string request_name = j["request"].get < string > ();
            helper.execute(request_name , j , client_fd);
        }
    }
    catch(exception &e)
    {
        LOG_ERROR(std::string("Invalid request received: ") + e.what());
        rem_fd(client_fd);
    }   
       
}

void init_users()
{
    int workers = architecture_utilities::get_sandbox_workers();

    for(int i = 1 ; i <= workers ; i++)
    {
        user_queue::get_instance().push(i);
    }
}

int main(int argc , char *argv[])
{
    if(argc != 4) handle_error(1 , "Provide IP PORT number of threads");

    read_args(argc , argv);
    create_threads();
    init_users();

    tests::run_tests();
    return 0;


    auto script_compiler = test_generation_script_compiler_task("swapsort", 1);

    script_compiler.execute(0, 1);

    auto new_problem = script_compiler.get_problem_metadata();
    auto output = new_problem;
    cout<<"Time limit: "<<output.time_limit<<"s\n";
    cout<<"Memory limit: "<<output.memory_limit<<"B\n";
    cout<<"Total points: "<<output.total_points<<'\n';
    cout<<"Subtask count: "<<output.group_count<<'\n';
    for(int i=0;i<output.groups.size();i++){
        cout<<"Group "<<i<<": "<<output.groups[i].total_points<<" points, "<<output.groups[i].test_count<<" tests\n";
    }
    cout<<"Test count: "<<output.test_count<<'\n';
    for(int i=0;i<output.tests.size();i++){
        cout<<"TEST "<<i<<'\n';
        cout<<"Groups: ";
        for(auto it : output.tests[i].groups){
            cout<<it<<' ';
        }
        cout<<'\n';
        cout<<"Generator args: ";
        for(auto it : output.tests[i].generator_args){
            cout<<it<<' ';
        }
        cout<<'\n';
        cout<<"Validator args: ";
        for(auto it : output.tests[i].validator_args){
            cout<<it<<' ';
        }
        cout<<'\n';
        cout<<"Main exe: "<<output.tests[i].main_path;
        cout<<'\n';
        cout<<"Checker args: ";
        for(auto it : output.tests[i].checker_args){
            cout<<it<<' ';
        }
        cout<<'\n';
        cout<<"Interactor args: ";
        for(auto it : output.tests[i].interactor_args){
            cout<<it<<' ';
        }
        cout<<"\n\n";
    }
    
    // if (system(command.c_str()) != 0){
    //     LOG_ERROR_USER(user_id, "Failed to create tests folder for problem " + problem_id + " rev " + std::to_string(rev_id));
    //     return result_enum::FAIL;
    // }
    new_problem.problem_status = problem_status_enum::NOT_EXISTS;
    problem_manager::get_instance().add_revision(new_problem);


    task_queue::get_instance().push(new preparator("swapsort", 1));
    // task_queue::get_instance().push(new generator_task("swapsort", 1));

    while(true){
      cout << general_utilities::enum_to_string(problem_manager::get_instance().get_problem_status("swapsort", 1)) << ' ' << problem_manager::get_instance().get_metadata("swapsort", 1).tests_to_generate_count << endl;
      sleep(1);
    }

    while(submission_manager::get_instance().size()>0){
    }
    LOG_INFO("All tests completed, exiting main thread");
    while(true);
    return 0;
}