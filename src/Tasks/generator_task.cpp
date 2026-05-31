#include <Tasks/generator_task.h>

result_enum generator_task::execute(pthread_t thread_id, int user_id){
  if (user_id <= 0){
    LOG_ERROR_USER(user_id, "Invalid user ID");
    problem_manager::get_instance().update_problem_status(problem_id , rev_id , problem_status_enum::FAILED);
    pending_submissions_manager::get_instance().problem_failed(problem_id, rev_id);
    return result_enum::FAIL;
  }
  if (architecture_utilities::clean_run_dir(user_id) != 0){
    LOG_ERROR_USER(user_id, "Failed to clean up run directory");
    problem_manager::get_instance().update_problem_status(problem_id , rev_id , problem_status_enum::FAILED);
    pending_submissions_manager::get_instance().problem_failed(problem_id, rev_id);
    return result_enum::FAIL;
  }
  
  LOG_INFO_USER(user_id, "Starting generator task execution for problem " + problem_id + ", revision " + std::to_string(rev_id));
  
  
  auto script_compiler = test_generation_script_compiler_task(problem_id, rev_id);

  result_enum aux_result = script_compiler.execute(thread_id, user_id);

  if (aux_result != result_enum::OK){
    LOG_ERROR_USER(user_id, "Test generation script compilation failed with result: " + general_utilities::enum_to_string(aux_result));
    //cine nu a bagat merita perna calda toata viata
    problem_manager::get_instance().update_problem_status(problem_id , rev_id , problem_status_enum::FAILED);
    pending_submissions_manager::get_instance().problem_failed(problem_id, rev_id);
    return aux_result;
  }

  if (script_compiler.get_failed()){
    LOG_ERROR_USER(user_id, "Test generation script compilation failed with message: " + script_compiler.get_compilation_message(100));
    problem_manager::get_instance().update_problem_status(problem_id , rev_id , problem_status_enum::FAILED);     //insane pull bug
    pending_submissions_manager::get_instance().problem_failed(problem_id, rev_id);
    return result_enum::FAIL;
  }
  
  problem_metadata prob_meta = script_compiler.get_problem_metadata();
  LOG_INFO_USER(user_id, "Test generation script compiled successfully, starting test generation for problem " + problem_id + ", revision " + std::to_string(rev_id) + " with " + std::to_string(prob_meta.test_count) + " tests to generate");
  


  
  problem_manager::get_instance().add_revision(prob_meta);

  
  std::string tests_path = architecture_utilities::get_problem_tests_folder(problem_id, rev_id);
  std::string inputs_path = architecture_utilities::get_problem_tests_inputs_folder(problem_id , rev_id);
  std::string correct_outputs_path = architecture_utilities::get_problem_tests_correct_outputs_folder(problem_id , rev_id);
  std::string problem_inputs_path = architecture_utilities::get_problem_inputs_folder(problem_id, rev_id);
  std::string problem_correct_outputs_path = architecture_utilities::get_problem_correct_outputs_folder(problem_id, rev_id);
  if (!std::filesystem::exists(problem_inputs_path) && mkdir(problem_inputs_path.c_str(), 0770)){
      LOG_ERROR_USER(user_id, "Failed to create inputs folder " + problem_inputs_path);
      problem_manager::get_instance().update_problem_status(problem_id , rev_id , problem_status_enum::FAILED);
      pending_submissions_manager::get_instance().problem_failed(problem_id, rev_id);
      return result_enum::FAIL;
  }
  if (!std::filesystem::exists(problem_correct_outputs_path) && mkdir(problem_correct_outputs_path.c_str(), 0770)){
      LOG_ERROR_USER(user_id, "Failed to create correct outputs folder " + problem_correct_outputs_path);
      problem_manager::get_instance().update_problem_status(problem_id , rev_id , problem_status_enum::FAILED);
      pending_submissions_manager::get_instance().problem_failed(problem_id, rev_id);
      return result_enum::FAIL;
  }
  if (std::filesystem::exists(tests_path))
  {
    for (int i = 0; i < prob_meta.test_count; i++){
      if (!general_utilities::copy_file(architecture_utilities::get_problem_test_input_path(problem_id, rev_id, i), architecture_utilities::get_problem_input_path(problem_id, rev_id, i), 0770)) goto extraction_failed;
      if (!general_utilities::copy_file(architecture_utilities::get_problem_test_correct_output_path(problem_id, rev_id, i), architecture_utilities::get_problem_correct_output_path(problem_id, rev_id, i), 0770)) goto extraction_failed;
    }

    LOG_INFO_USER(user_id, "Test data for problem " + problem_id + ", revision " + std::to_string(rev_id) + " already exists, skipping test generation");
    problem_manager::get_instance().update_problem_status(problem_id , rev_id , problem_status_enum::DONE);
    pending_submissions_manager::get_instance().problem_done(problem_id, rev_id);
    return result_enum::OK;
    extraction_failed:
    if (!rmdir(tests_path.c_str())){
        LOG_ERROR_USER(user_id, "Failed to remove tests folder " + tests_path);
        problem_manager::get_instance().update_problem_status(problem_id , rev_id , problem_status_enum::FAILED);
        pending_submissions_manager::get_instance().problem_failed(problem_id, rev_id);
        return result_enum::FAIL;
    }
    if (!rmdir(inputs_path.c_str())){
        LOG_ERROR_USER(user_id, "Failed to remove inputs folder " + inputs_path);
        problem_manager::get_instance().update_problem_status(problem_id , rev_id , problem_status_enum::FAILED);
        pending_submissions_manager::get_instance().problem_failed(problem_id, rev_id);
        return result_enum::FAIL;
    }
    if (!rmdir(correct_outputs_path.c_str())){
        LOG_ERROR_USER(user_id, "Failed to remove correct outputs folder " + correct_outputs_path);
        problem_manager::get_instance().update_problem_status(problem_id , rev_id , problem_status_enum::FAILED);
        pending_submissions_manager::get_instance().problem_failed(problem_id, rev_id);
        return result_enum::FAIL;
    }
    if (!rmdir(problem_inputs_path.c_str())){
        LOG_ERROR_USER(user_id, "Failed to remove problem inputs folder " + problem_inputs_path);
        problem_manager::get_instance().update_problem_status(problem_id , rev_id , problem_status_enum::FAILED);
        pending_submissions_manager::get_instance().problem_failed(problem_id, rev_id);
        return result_enum::FAIL;
    }
    if (!rmdir(problem_correct_outputs_path.c_str())){
        LOG_ERROR_USER(user_id, "Failed to remove problem correct outputs folder " + problem_correct_outputs_path);
        problem_manager::get_instance().update_problem_status(problem_id , rev_id , problem_status_enum::FAILED);
        pending_submissions_manager::get_instance().problem_failed(problem_id, rev_id);
        return result_enum::FAIL;
    }
  }

  if (!std::filesystem::exists(tests_path) && mkdir(tests_path.c_str(), 0770)){
      LOG_ERROR_USER(user_id, "Failed to create tests folder " + tests_path);
      problem_manager::get_instance().update_problem_status(problem_id , rev_id , problem_status_enum::FAILED);
      pending_submissions_manager::get_instance().problem_failed(problem_id, rev_id);
      return result_enum::FAIL;
  }
  if (!std::filesystem::exists(inputs_path) && mkdir(inputs_path.c_str(), 0770)){
      LOG_ERROR_USER(user_id, "Failed to create inputs folder " + inputs_path);
      problem_manager::get_instance().update_problem_status(problem_id , rev_id , problem_status_enum::FAILED);
      pending_submissions_manager::get_instance().problem_failed(problem_id, rev_id);
      return result_enum::FAIL;
  }
  if (!std::filesystem::exists(correct_outputs_path) && mkdir(correct_outputs_path.c_str(), 0770)){
      LOG_ERROR_USER(user_id, "Failed to create correct outputs folder " + correct_outputs_path);
      problem_manager::get_instance().update_problem_status(problem_id , rev_id , problem_status_enum::FAILED);
      pending_submissions_manager::get_instance().problem_failed(problem_id, rev_id);
      return result_enum::FAIL;
  }
  if (!std::filesystem::exists(problem_inputs_path) && mkdir(problem_inputs_path.c_str(), 0770)){
      LOG_ERROR_USER(user_id, "Failed to create problem inputs folder " + problem_inputs_path);
      problem_manager::get_instance().update_problem_status(problem_id , rev_id , problem_status_enum::FAILED);
      pending_submissions_manager::get_instance().problem_failed(problem_id, rev_id);
      return result_enum::FAIL;
  }
  if (!std::filesystem::exists(problem_correct_outputs_path) && mkdir(problem_correct_outputs_path.c_str(), 0770)){
      LOG_ERROR_USER(user_id, "Failed to create problem correct outputs folder " + problem_correct_outputs_path);
      problem_manager::get_instance().update_problem_status(problem_id , rev_id , problem_status_enum::FAILED);
      pending_submissions_manager::get_instance().problem_failed(problem_id, rev_id);
      return result_enum::FAIL;
  }

  LOG_INFO_USER(user_id, "Test data was either not found or extracted unsuccessfully for problem " + problem_id + ", revision " + std::to_string(rev_id) + ", starting test generation");

 
  problem_manager::get_instance().start_generating_tests(problem_id, rev_id);

 
  for (int i = 0; i < prob_meta.test_count; i++){

    task_queue::get_instance().push(new single_test_generator_task(problem_id, rev_id, i, prob_meta.tests[i], (long long)(prob_meta.memory_limit), prob_meta.time_limit));
  }

 
  return result_enum::OK;
}