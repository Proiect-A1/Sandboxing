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

    system((std::string("unzip -d ") + architecture_utilities::get_problem_data_folder(problem_id , rev_id) + " " + "-u " + path + " 2>&1 > /dev/null").c_str());
    LOG_DEBUG("Unzip finished");
    // trebe verificat daca o venit cu testele
    
    std::string tests_path = architecture_utilities::get_problem_tests_folder(problem_id , rev_id);
    std::string inputs_path = architecture_utilities::get_problem_tests_inputs_folder(problem_id , rev_id);
    std::string correct_outputs_path = architecture_utilities::get_problem_tests_correct_outputs_folder(problem_id , rev_id);
    std::string problem_inputs_path = architecture_utilities::get_sandbox_path() + "/inputs/" + problem_id + "." + std::to_string(rev_id);
    std::string problem_correct_outputs_path = architecture_utilities::get_sandbox_path() + "/correct_outputs/" + problem_id + "." + std::to_string(rev_id);
    mkdir(tests_path.c_str(), 0700);
    mkdir(inputs_path.c_str(), 0700);
    mkdir(correct_outputs_path.c_str(), 0700);
    mkdir(problem_inputs_path.c_str(), 0700);
    mkdir(problem_correct_outputs_path.c_str(), 0700);

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
        LOG_WARNING(std::string("bla bla") + folder);

        if (std::filesystem::exists(folder)) {
            for (const auto& entry : std::filesystem::directory_iterator(folder)) {
                if (entry.is_regular_file() && entry.path().extension() == ".cpp") {
                    sources_to_compile.push_back(entry.path().string());
                }
            }
        }
      }
    
    pm.start_compiling_sources(problem_id, rev_id, sources_to_compile.size());

    for (auto source : sources_to_compile){
        task_queue::get_instance().push(new problem_compiler_task(problem_id, rev_id, source));
        LOG_WARNING(std::string("HERERHERHEREHRHERHERHERHEHREHREHRE        ") + source);
    }

    
    return result_enum::OTHER; //n-ar trebui sa returneze nimic
}

bool preparator::check_permissions(int user_id) 
{
    (void)user_id;
    return true; //trebuie implementat ig
}
