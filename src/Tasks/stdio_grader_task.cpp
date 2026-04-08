#include <Tasks/stdio_grader_task.h>
result_enum stdio_grader_task::execute(int thread_id, int user_id){
  if(!check_permissions()){
    print_error(thread_id, user_id, "Permission check failed");
    return result_enum::FAIL;
  }
  if(user_id <= 0){
    print_error(thread_id, user_id, "Invalid user ID");
    return result_enum::FAIL;
  }

  problem_manager& pm = problem_manager::get_instance();
  submission_manager& sm = submission_manager::get_instance();
  /*std::unique_ptr<task> stdio_runner_factory(
    language_enum language,
    long submission_id,
    std::string input_path,
    std::string output_path,
    float run_time_limit,
    long run_memory_limit,
    uint8_t priority = 0);*/

  problem_metadata problem = pm.get_metadata(problem_id, rev_id);
  problem.time_limit = 3000;
  problem.memory_limit = 1024ll * 1024 * 50; // 50MB
  // submission_data submission = sm.get_submission(submission_id);
  submission_data submission;
  submission.language = language_enum::CPP;
  utilities::change_dir_to_user("amarat" + std::to_string(user_id));

  std::string submission_exec_path = submission_info_utilities::get_submission_exec_path(submission_id);
  std::string problem_input_path = submission_info_utilities::get_problem_input_path(problem_id, rev_id, test);
  std::string problem_correct_output_path = submission_info_utilities::get_problem_correct_output_path(problem_id, rev_id, test);
  
  std::string input_path = "input";
  std::string output_path = "output";
  std::string exec_path = "main_exec";
  std::string correct_output_path = "correct_output";
  std::string username = "amarat" + std::to_string(user_id);

  utilities::change_dir_to_user(username);
  
  if (!utilities::copy_file(submission_exec_path, exec_path, 0755)){
    print_error(thread_id, user_id, "Couldn't copy submission_exec to run directory");
    return result_enum::FAIL;
  }

  if (!utilities::copy_file(problem_input_path, input_path, 0644)){
    print_error(thread_id, user_id, "Couldn't copy problem input to run directory");
    return result_enum::FAIL;
  }

  auto runner_task = stdio_runner_factory(
    submission.language,
    submission_id,
    exec_path,
    input_path,
    output_path,
    problem.time_limit,
    problem.memory_limit,
    0);
  if(runner_task == nullptr){
    print_error(thread_id, user_id, "Failed to create runner task");
    return result_enum::FAIL;
  }


  utilities::change_dir_to_sandbox();

  result_enum result = runner_task->execute(thread_id, user_id);
  if (result == result_enum::FAIL)
  {
    print_error(thread_id, user_id, "Runner task execution failed");
    return result_enum::FAIL;
  }
  if (result != result_enum::OK)
  {
    return result;
  }

  // de checkuit in pula mea

  // checkuim doar cu diff se va mai baga altceva
  
  utilities::change_dir_to_user(username);

  if (!utilities::copy_file(problem_correct_output_path, correct_output_path, 0644)){
    print_error(thread_id, user_id, "Couldn't copy problem correct output to run directory");
    return result_enum::FAIL;
  }

  checker_task checker(input_path, output_path, correct_output_path, "");

  if (checker.execute(thread_id, user_id) != result_enum::OK){
    print_error(thread_id, user_id, "Checker task execution failed");
    return result_enum::FAIL;
  }

  if (checker.get_point_percentage() == 1){
    result = result_enum::OK;
  } else if (checker.get_point_percentage() > 0){
    result = result_enum::PA;
  } else {
    result = result_enum::WA;
  }
  // system("pwd");
  // system(("cat " + output_path).c_str());
  // system(("cat " + correct_output_path).c_str());
  system("rm -rf *");

  return result;
}


void stdio_grader_task:: print_error(int thread_id, int user_id,const std::string& message){
  fprintf(stderr, "STDIO_Grader task running on thread %d, with user %d: %s\n", thread_id, user_id, message.c_str());
}