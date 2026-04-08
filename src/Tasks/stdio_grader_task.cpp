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

  problem_metadata problem = pm.get_metadata(problem_id, rev_id);
  submission_data submission = sm.get_submission(submission_id);

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


  submission_test test_result = submission_manager::get_instance().get_submission(submission_id).tests[test];
  test_result.result = runner_task->execute(thread_id, user_id);
  test_result.time_used = runner_task->get_time_consumed();
  test_result.memory_used = runner_task->get_memory_consumed();
  
  if (test_result.result == result_enum::FAIL)
  {
    print_error(thread_id, user_id, "Runner task execution failed");
    return result_enum::FAIL;
  }

  if (test_result.result != result_enum::OK)
  {
    test_result.points = 0;
    test_result.result = test_result.result;
    sm.add_completed_test(submission_id, test, test_result);
    return test_result.result;
  }
  
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
    test_result.points = 1;
    test_result.result = result_enum::OK;
  } else if (checker.get_point_percentage() > 0){
    test_result.points = 0.5;
    test_result.result = result_enum::PA;
  } else {
    test_result.points = 0;
    test_result.result = result_enum::WA;
  }
  
  test_result.message = checker.get_message();
  
  // system("pwd");
  // system(("cat " + output_path).c_str());
  // system(("cat " + correct_output_path).c_str());
  system("rm -rf *");

  // print_error(thread_id, user_id, "Test " + std::to_string(test) + " completed with result " + checker.get_message() + ", points: " + std::to_string(test_result.points) + ", time used: " + std::to_string(test_result.time_used) + " ms, memory used: " + std::to_string(test_result.memory_used) + " B");

  sm.add_completed_test(submission_id, test, test_result);
  
  return test_result.result;
}


void stdio_grader_task:: print_error(int thread_id, int user_id,const std::string& message){
  fprintf(stderr, "STDIO_Grader task running on thread %d, with user %d: %s\n", thread_id, user_id, message.c_str());
}