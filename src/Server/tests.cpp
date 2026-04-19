
#include <Server/IO.hpp>
#include <fcntl.h>
#include <Server/tests.hpp>
#include <sys/stat.h> 
#include <Server/exceptions.hpp>
#include <Tasks/evaluator_task.h>
#include <Tasks/stdio_grader_task.h>
#include <iostream>
#include <Utilities/general_utilities.h>
#include <Singletoni/problem_manager.h>
#include <Singletoni/submission_manager.h>
#include <vector>
#include <Singletoni/user_queue.h>
#include <Singletoni/task_queue.h>
#include <pthread.h>

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
                LOG_DEBUG(std::string("here") + f.what());
                throw;
            }
            
        }
        catch(std::exception &e)
        {
            LOG_ERROR(e.what());
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

    void test_problem_evaluation_protocol()
    {        
        system("mkdir $SANDBOX_PATH/submissions/12345 2> /dev/null");
        system("touch $SANDBOX_PATH/submissions/12345/main.cpp 2> /dev/null");
        system("echo '#include <iostream> \n int main(){int x; std::cin >> x; std::cout << x << std::endl;return 0;}' > $SANDBOX_PATH/submissions/12345/main.cpp");

        string submission_id = "12345";

        system("mkdir $SANDBOX_PATH/inputs/abcde.12 2> /dev/null");
        system("mkdir $SANDBOX_PATH/correct_outputs/abcde.12 2> /dev/null");

        string problem_id = "abcde";
        string problem_ver_id = "abcde.12";
        int rev_id = 12;

        for(int i = 0 ; i < 10 ; i++)
        {
            char comm[1000];
            sprintf(comm , "echo %d > $SANDBOX_PATH/inputs/%s/00%d.in" , i , problem_ver_id.c_str() , i);
            system(comm);
            sprintf(comm , "echo %d > $SANDBOX_PATH/correct_outputs/%s/00%d.ok" , i , problem_ver_id.c_str() , i);
            system(comm);   
        }

        problem_manager& pm = problem_manager::get_instance();
        submission_manager& sm = submission_manager::get_instance();

        problem_metadata new_problem;
        new_problem.problem_id = problem_id;
        new_problem.rev_id = rev_id;
        new_problem.group_count = 10;
        new_problem.test_count = 10;
        new_problem.time_limit = 30000;
        new_problem.memory_limit = 1024ll * 1024 * 1024 * 3; // 15 GB
        new_problem.total_points = 100.0;
        new_problem.problem_status = problem_status_enum::DONE;
        new_problem.groups = std::vector<group_metadata>(10, {10.0, 1, group_type_enum::GROUP_MIN});
        new_problem.tests = std::vector<test_metadata>(10, {"", "", "", {}});
        for (int i = 0; i < 10; i++){
            new_problem.tests[i].groups.push_back(i);
        }

        pm.add_revision(new_problem);

        for (int i = 1; i <= 10; i++){
            architecture_utilities::change_dir_to_user(i);
            system("rm -f *");
            user_queue::get_instance().push(i);
        }

        sm.insert(submission_id, language_enum::CPP, problem_id , rev_id , 1);

        submission_data submission = sm.get_submission(submission_id);


        evaluator_task* eva = new evaluator_task(submission_id , problem_id , rev_id);

        task_queue::get_instance().push(eva);
        // sleep(20);
    }
}