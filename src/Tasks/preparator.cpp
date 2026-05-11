#include <Tasks/preparator.h>
#include <Utilities/architecture_utilities.h>
#include <Singletoni/problem_manager.h>
#include <sys/wait.h>

preparator::preparator(std::string problem_id , int rev_id)
{
    this -> problem_id = problem_id;
    this -> rev_id = rev_id;
}

result_enum preparator::execute(pthread_t thread_id, int user_id)
{
    std::string path = architecture_utilities::get_sandbox_path() + "/tmp/" + problem_id + "." + std::to_string(rev_id);
    int pid = fork();

    if(pid == 0)
    {
        execlp("unzip" , "unzip" , "-d" , architecture_utilities::get_problem_data_folder(problem_id , rev_id).c_str() , path.c_str() , NULL);
        //("unzip -d " + architecture_utilities::get_problem_data_folder(problem_id , rev_id) + " " + path + " 2>&1 > /dev/null").c_str());
        LOG_ERROR("execlp failed");
        exit(0);
    }

    wait(&pid);

    problem_manager &pm = problem_manager::get_instance();
    pm.update_problem_status(problem_id , rev_id , problem_status_enum::DOWNLOADED); //add generator
    pm.update_problem_status(problem_id , rev_id , problem_status_enum::DONE);
    
    return result_enum::OTHER; //n-ar trebui sa returneze nimic
}

bool preparator::check_permissions(int user_id) 
{
    (void)user_id;
    return true; //trebuie implementat ig
}
