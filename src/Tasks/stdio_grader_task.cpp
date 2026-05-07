#include <Tasks/stdio_grader_task.h>


stdio_grader_task::stdio_grader_helper::~stdio_grader_helper() {
  submission_manager& sm = submission_manager::get_instance();
  if (system(("rm -rf " + architecture_utilities::get_run_dir_absolute_path(user_id)+ "/*").c_str()) != 0){
    LOG_ERROR_USER(user_id, "Failed to clean up run directory from helper");
    test_result = result_enum::FAIL;
  }
  else {
    LOG_INFO_USER(user_id, "Successfully cleaned up run directory from helper");
  }
  sm.add_completed_test(submission_id, test_id, test);
  LOG_INFO_USER(user_id, "Submission " + submission_id + "Test " + std::to_string(test_id) + " completed with result " + test.message + ", points: " + std::to_string(test.points) + ", time used: " + std::to_string(test.time_used) + " ms, memory used: " + std::to_string(test.memory_used) + " B");
}

result_enum stdio_grader_task::execute(pthread_t thread_id, int user_id){

  stdio_grader_helper helper(result_enum::FAIL, submission_id);
  helper.thread_id = thread_id;
  helper.user_id = user_id;
  helper.test_id = test_id;
  
  helper.test = submission_manager::get_instance().get_submission(submission_id).tests[test_id];

  if(!check_permissions(user_id)){
    LOG_ERROR_USER(user_id, "Permission check failed");
    return result_enum::FAIL;
  }
  if(user_id <= 0){
    LOG_ERROR_USER(user_id, "Invalid user ID");
    return result_enum::FAIL;
  }
  if (system(("rm -rf " + architecture_utilities::get_run_dir_absolute_path(user_id)+ "/*").c_str()) != 0){
    LOG_ERROR_USER(user_id, "Failed to clean up run directory");
    return result_enum::FAIL;
  }
  else{
    LOG_INFO_USER(user_id, "Successfully cleaned up run directory");
  }

  problem_manager& pm = problem_manager::get_instance();
  submission_manager& sm = submission_manager::get_instance();

  problem_metadata problem = pm.get_metadata(problem_id, rev_id);
  if (problem.problem_id.empty()){
    LOG_ERROR_USER(user_id, "Problem metadata not found in manager. Problem ID: " + problem_id + ", Rev ID: "+ std::to_string(rev_id));
    return result_enum::FAIL;
  }
  if (!sm.count(submission_id)){
    LOG_ERROR_USER(user_id, "Submission data not found in manager. Submission ID: " + submission_id);
    return result_enum::FAIL;
  }
  submission_data submission = sm.get_submission(submission_id);
  
  std::string submission_exec_path = architecture_utilities::get_submission_exec_path(submission_id);
  std::string problem_input_path = architecture_utilities::get_problem_input_path(problem_id, rev_id, test_id);
  std::string problem_correct_output_path = architecture_utilities::get_problem_correct_output_path(problem_id, rev_id, test_id);
  
  std::string input_path = "input";
  std::string output_path = "output";
  std::string exec_path = "main_exec";
  std::string correct_output_path = "correct_output";
  std::string username = architecture_utilities::get_weak_user(user_id);


  if (!general_utilities::copy_file(submission_exec_path, architecture_utilities::get_run_dir_absolute_path(user_id) + "/" + exec_path, 0755)){

    LOG_ERROR_USER(user_id, "Couldn't copy submission_exec to run directory. Submission ID: " + submission_id + " " + submission_exec_path + " " + general_utilities::syscall_to_string("pwd") + general_utilities::syscall_to_string("whoami"));
    return result_enum::FAIL;
  }

  if (!general_utilities::copy_file(problem_input_path, architecture_utilities::get_run_dir_absolute_path(user_id) + "/" + input_path, 0644)){
    LOG_ERROR_USER(user_id, "Couldn't copy problem input to run directory. Problem ID: " + problem_id + ", Rev ID: "+ std::to_string(rev_id) + ", Test ID: " +  std::to_string(test_id) + ", Problem_input_path:" + problem_input_path + ", " + ", Destination_input_path: " + (architecture_utilities::get_run_dir_absolute_path(user_id) + "/" + input_path) + general_utilities::syscall_to_string("pwd") + general_utilities::syscall_to_string("whoami"));
    return result_enum::FAIL;
  }

  LOG_INFO_USER(user_id, "Current status: " + general_utilities::syscall_to_string("whoami") + general_utilities::syscall_to_string("ls " + architecture_utilities::get_run_dir_absolute_path(user_id)) + general_utilities::syscall_to_string("pwd"));

  auto runner_task = runner_factories::stdio_submission_runner_factory[submission.language](
    submission_id,
    exec_path,
    input_path,
    output_path,
    problem.time_limit,
    problem.memory_limit,
    0);
  if(runner_task == nullptr){
    LOG_ERROR_USER(user_id, "Failed to create runner task");
    return result_enum::FAIL;
  }

  
  LOG_INFO_USER(user_id, "Starting runner task execution. Submission ID: " + submission_id + ", Problem ID: " + problem_id + ", Rev ID: "+ std::to_string(rev_id) + ", Test ID: " + std::to_string(test_id) + general_utilities::syscall_to_string("ls " + architecture_utilities::get_run_dir_absolute_path(user_id)) + general_utilities::syscall_to_string("whoami"));
  helper.test_result = runner_task->execute(thread_id, user_id);
  helper.test.time_used = runner_task->get_time_consumed();
  helper.test.memory_used = runner_task->get_memory_consumed();

  if (helper.test_result == result_enum::FAIL)
  {
    LOG_ERROR_USER(user_id, "Runner task execution failed");
    return result_enum::FAIL;
  }

  if (helper.test_result != result_enum::OK){
    helper.test.points = 0;
    helper.test.result = helper.test_result;
    return helper.test.result;
  }

  if (!general_utilities::copy_file(problem_correct_output_path, ::architecture_utilities::get_run_dir_absolute_path(user_id) + "/" + correct_output_path, 0644)){
    LOG_ERROR_USER(user_id, "Couldn't copy problem correct output to run directory. Problem ID: " + problem_id + ", Rev ID: "+ std::to_string(rev_id) + ", Test ID: " + std::to_string(test_id));
    return result_enum::FAIL;
  }
  
  checker_task checker(submission_id, input_path, output_path, correct_output_path, "");
  
  if (checker.execute(thread_id, user_id) != result_enum::OK){
    LOG_ERROR_USER(user_id, "Checker task execution failed");
    return result_enum::FAIL;
  }
  
  if (checker.get_point_percentage() == 1){
    helper.test.points = 1;
    helper.test.result = result_enum::OK;
  } else if (checker.get_point_percentage() > 0){
    helper.test.points = 0.5;
    helper.test.result = result_enum::PA;
  } else {
    helper.test.points = 0;
    helper.test.result = result_enum::WA;
  }
  
  helper.test.message = checker.get_message();
  
  return helper.test.result;
}
