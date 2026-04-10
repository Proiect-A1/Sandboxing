
#include "../headers/IO.hpp"
#include <fcntl.h>
#include "../headers/tests.hpp"
#include <sys/stat.h> 
#include "../headers/exceptions.hpp"

namespace tests 
{
    void test_done_test_request()
    {
        IO::done_test_request("submissionId" , 10 , 5 , "wrong answer" , 10.4 , 100.0 , 14.5 , 1000 , 1000);
    }

    void test_done_subtask_request()
    {
        IO::done_subtask_request("submissionId" , 10 , 100 , 100.5  , 10.4 , 1000 , 1000);
    }

    void test_done_submission_request()
    {
        IO::done_submission_request("submissionId" , 10 , 100 , 10.4 , 1000 , 100);
    }


    void test_upload_tests_request()
    {
        IO helper;
        int fd = open("arch.zip" , O_RDONLY);
        helper.upload_tests_request("problemId" , 10 , "zip" , {100 , {100}} , fd);
    }

    void test_pull_problem_request()
    {
        IO::pull_problem_request("problemId");
    }

    void run_tests()
    {
        test_done_submission_request();
        test_done_subtask_request();
        test_done_test_request();
        test_upload_tests_request();
        test_pull_problem_request();
    }

    void test_create_folder()
    {
        if(mkdir("./sandbox/submissions/chestie" , 0700) == -1) handle_error(1 , "no folder :/");
        if(rmdir("./sandbox/submissions/chestie") == -1) handle_error(1 , "no rmdir folder :/");
    }

    void test_create_file()
    {
        if(open("/home/sochu/Documents/Projects/sandbox/sandbox/submissions/subm.txt" , O_CREAT | O_TRUNC | O_RDWR , 0600) == -1) handle_error(1 , "no file :/");
        if(remove("/home/sochu/Documents/Projects/sandbox/sandbox/submissions/subm.txt") == -1) handle_error(1 , "no rm file :/")
    }

    void test_evaluate_request()
    {
        IO helper;
        json j = {{"submissionId" , "10"} , {"language" , "cpp"}};
        system("rm -r sandbox/submissions/10");
        helper.evaluate_request(j , -1);
        system("rm -r sandbox/submissions/10");
    }

    void test_throws()
    {
        try 
        {
            try 
            {
                throw unfinished_request_exception();
            }
            catch(exception &e)
            {
                exception &f = e;
                cerr << endl << "here" << f.what() << endl;
                throw;
            }
            
        }
        catch(std::exception &e)
        {
            cerr << e.what();
            fflush(stderr);
        }
    }

    void test_send_problem_request()
    {
        IO helper;
        json j = {{"problemId" , "10"} , {"revId" , 10}};
        system("rm -r sandbox/submissions/10");
        helper.send_problem_request(j , -1);
        system("rm -r sandbox/submissions/10");
    }
}