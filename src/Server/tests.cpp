
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
        // test_done_submission_request();
        // test_done_subtask_request();
        // test_done_test_request();
        // test_upload_tests_request();
        // test_pull_problem_request();
        register_problem_expresie();
        register_problem_abcde();
        test_problem_evaluation_protocol();
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
        //test_submission("12345_1", "abcde", 12);
        //test_submission("12345_2", "abcde", 12);
        //test_submission("12345_3", "abcde", 12);

        //test_submission("1000", "expresie", 1);
        //test_submission("1001", "expresie", 1);
        //test_submission("1002", "expresie", 1);
        //test_submission("1003", "expresie", 1);
        // test_submission("1004", "expresie", 1);
        // test_submission("1005", "expresie", 1);
         test_submission("1009", "expresie", 1);
        // test_submission("1010", "expresie", 1);
        // test_submission("1011", "expresie", 1);
        // test_submission("1012", "expresie", 1);
        // test_submission("500", "expresie", 1);
        // test_submission("501", "abcde", 12);
        // test_submission("502", "abcde", 12);
        // test_submission("503", "abcde", 12);
        // test_submission("200", "abcde", 12);
    }

    void register_problem_abcde()
    {
        string problem_id = "abcde";
        string problem_ver_id = "abcde.12";
        int rev_id = 12;

        // generez teste
        system("mkdir $SANDBOX_PATH/inputs/abcde.12 2> /dev/null");
        system("mkdir $SANDBOX_PATH/correct_outputs/abcde.12 2> /dev/null");

        for(int i = 0 ; i < 10 ; i++)
        {
            char comm[1000];
            sprintf(comm , "echo %d > $SANDBOX_PATH/inputs/%s/00%d.in" , i , problem_ver_id.c_str() , i);
            system(comm);
            sprintf(comm , "echo %d > $SANDBOX_PATH/correct_outputs/%s/00%d.ok" , i , problem_ver_id.c_str() , i);
            system(comm);   
        }

        problem_metadata new_problem;
        new_problem.problem_id = problem_id;
        new_problem.rev_id = rev_id;
        new_problem.group_count = 10;
        new_problem.test_count = 10;
        new_problem.time_limit = 30000;
        new_problem.memory_limit = 1024ll * 1024 * 64; // 64 MB
        new_problem.total_points = 100.0;
        new_problem.problem_status = problem_status_enum::DONE;
        new_problem.groups = std::vector<group_metadata>(10, {10.0, 1, group_type_enum::GROUP_MIN});
        new_problem.tests = std::vector<test_metadata>(10, {"", "", "", {}});
        for (int i = 0; i < 10; i++){
            new_problem.tests[i].groups.push_back(i);
        }

        problem_manager& pm = problem_manager::get_instance();
        pm.add_revision(new_problem);
    }

    void register_problem_expresie()
    {
        string problem_id = "expresie";
        string problem_ver_id = "expresie.1";
        int rev_id = 1;

        system("cp -r testing_data/expresie/inputs/expresie.1 sandbox/inputs");
        system("cp -r testing_data/expresie/correct_outputs/expresie.1 sandbox/correct_outputs");
        
        problem_metadata new_problem;
        new_problem.problem_id = problem_id;
        new_problem.rev_id = rev_id;
        new_problem.group_count = 6;
        new_problem.test_count = 6;
        new_problem.time_limit = 9000; // 9 secunde
        new_problem.memory_limit = 1024ll * 1024 * 64; // 64 MB
        new_problem.total_points = 100.0;
        new_problem.problem_status = problem_status_enum::DONE;
        new_problem.groups = std::vector<group_metadata>{
            { 0, 1, group_type_enum::GROUP_MIN},
            {20, 1, group_type_enum::GROUP_MIN},
            {20, 1, group_type_enum::GROUP_MIN},
            {20, 1, group_type_enum::GROUP_MIN},
            {20, 1, group_type_enum::GROUP_MIN},
            {20, 1, group_type_enum::GROUP_MIN}
        };
        new_problem.tests = std::vector<test_metadata>(6, {"", "", "", {}});
        for (int i = 0; i < 6; i++){
            new_problem.tests[i].groups.push_back(i);
        }

        problem_manager& pm = problem_manager::get_instance();
        pm.add_revision(new_problem);
    }

    // reads the cpp file from $SANDBOX_PATH/../testing_data/submissions
    void test_submission(string submission_id, string problem_id, int rev_id)
    {
        cout << "======= TEST SUBMISSION: id=" << submission_id << " problem=" << problem_id << '.' << rev_id << "=======\n";
        fflush(stdout);
        fflush(stderr);

        system(("rm -rf $SANDBOX_PATH/submissions/" + submission_id + " 2> /dev/null").c_str());
        system(("mkdir $SANDBOX_PATH/submissions/" + submission_id).c_str());
        system((
            "cp $SANDBOX_PATH/../testing_data/submissions/" + submission_id + "_*.cpp " +
            "$SANDBOX_PATH/submissions/" + submission_id + "/main.cpp"
        ).c_str());
        
        for (int i = 1; i <= 10; i++){
            architecture_utilities::change_dir_to_user(i);
            system("rm -f *");
            user_queue::get_instance().push(i);
        }

        submission_manager& sm = submission_manager::get_instance();
        sm.insert(submission_id, language_enum::CPP, problem_id , rev_id , 1);

        submission_data submission = sm.get_submission(submission_id);

        evaluator_task* eva = new evaluator_task(submission_id , problem_id , rev_id);

        task_queue::get_instance().push(eva);

        //while (sm.is_done(submission_id) == 0);
    }
}