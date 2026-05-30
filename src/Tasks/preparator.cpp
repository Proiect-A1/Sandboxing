#include <Tasks/preparator.h>

preparator::preparator(std::string problem_id , int rev_id)
{
    this -> problem_id = problem_id;
    this -> rev_id = rev_id;
}

result_enum preparator::execute(pthread_t thread_id, int user_id)
{

    std::string path = architecture_utilities::get_problem_zip(problem_id , rev_id);
    // int pid = fork();

    // if(pid == 0)
    // {
    //     std::string aux_str = architecture_utilities::get_problem_data_folder(problem_id , rev_id);
    //     execlp("unzip" , "unzip" , "-d" , aux_str.c_str() , path.c_str() , NULL);
    //     //("unzip -d " + architecture_utilities::get_problem_data_folder(problem_id , rev_id) + " " + path + " 2>&1 > /dev/null").c_str());
    //     LOG_ERROR("execlp failed");
    //     exit(0);
    // }

    // wait(&pid);
    if(system((std::string("unzip -d ") + architecture_utilities::get_problem_data_folder(problem_id , rev_id) + " " + "-o " + "-u " + path + " 2>&1 > /dev/null").c_str()))
    {
        LOG_ERROR("unzip error");
        problem_manager::get_instance().update_problem_status(problem_id , rev_id , problem_status_enum::FAILED);
        return result_enum::FAIL;
    }

    if(system((std::string("rm -rf ") + path).c_str()))
    {
        LOG_ERROR("couldn't delete old archive");
        problem_manager::get_instance().update_problem_status(problem_id , rev_id , problem_status_enum::FAILED);
        return result_enum::FAIL;
    }
   
    

    problem_manager &pm = problem_manager::get_instance();
    pm.update_problem_status(problem_id , rev_id , problem_status_enum::DOWNLOADED); //add generator
    std::cerr << "in the prep time: " << problem_manager::get_instance().get_metadata(problem_id , rev_id).founding_submission_id << "   problem_id  " << problem_manager::get_instance().get_metadata(problem_id , rev_id).problem_id << "   " <<problem_manager::get_instance().get_metadata(problem_id , rev_id).rev_id << std::endl;
    

    std::vector<std::string> folders_to_search;
    std::vector<std::string> sources_to_compile;
    folders_to_search.push_back(architecture_utilities::get_problem_sources_folder(problem_id, rev_id));
    folders_to_search.push_back(architecture_utilities::get_problem_checkers_folder(problem_id, rev_id));
    folders_to_search.push_back(architecture_utilities::get_problem_validators_folder(problem_id, rev_id));
    folders_to_search.push_back(architecture_utilities::get_problem_generators_folder(problem_id, rev_id));
    folders_to_search.push_back(architecture_utilities::get_problem_interactors_folder(problem_id, rev_id));

    for (auto folder : folders_to_search){

        if (std::filesystem::exists(folder)) {
            for (const auto& entry : std::filesystem::directory_iterator(folder)) {
                if (entry.is_regular_file() && entry.path().extension() == ".cpp") {
                    sources_to_compile.push_back(entry.path().string());
                }
            }
        }
      }
    
    if (!sources_to_compile.empty()){
      pm.start_compiling_sources(problem_id, rev_id, sources_to_compile.size());

      for (auto source : sources_to_compile){
          task_queue::get_instance().push(new problem_compiler_task(problem_id, rev_id, source));
      }
    }
    else{
      task_queue::get_instance().push(new generator_task(problem_id, rev_id));
    }

    
    return result_enum::OK; //returneaza ca a mers bine task-ul
}

bool preparator::check_permissions(int user_id) 
{
    (void)user_id;
    return true; //trebuie implementat ig
}
