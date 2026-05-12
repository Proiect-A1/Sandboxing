#include <Tasks/generator_task.hpp>

result_enum generator_task::execute(pthread_t thread_id, int user_id){
  if (user_id <= 0){
    LOG_ERROR_USER(user_id, "Invalid user ID");
    return result_enum::FAIL;
  }
  if (architecture_utilities::clean_run_dir(user_id) != 0){
    LOG_ERROR_USER(user_id, "Failed to clean up run directory");
    return result_enum::FAIL;
  }
  
  LOG_DEBUG_USER(user_id, "Starting generator task execution for problem " + problem_id + ", revision " + std::to_string(rev_id));
  



  
  auto script_compiler = test_generation_script_compiler_task(problem_id, rev_id);

  result_enum aux_result = script_compiler.execute(thread_id, user_id);

  if (aux_result != result_enum::OK){
    LOG_ERROR_USER(user_id, "Test generation script compilation failed with result: " + general_utilities::enum_to_string(aux_result));
    return aux_result;
  }

  if (script_compiler.get_failed()){
    LOG_ERROR_USER(user_id, "Test generation script compilation failed with message: " + script_compiler.get_compilation_message(100));
    return result_enum::FAIL;
  }

  problem_metadata prob_meta = script_compiler.get_problem_metadata();

  

  for (int i = 0; i < prob_meta.test_count; i++){

    task_queue::get_instance().push(new single_test_generator_task(problem_id, rev_id, 0, prob_meta.tests[i], (long long)(prob_meta.memory_limit), prob_meta.time_limit));
  }

  return result_enum::OK;
}