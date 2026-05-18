#include <Tasks/stdio_grader_task.h>


stdio_grader_task::stdio_grader_helper::~stdio_grader_helper() {
  submission_manager& sm = submission_manager::get_instance();
  if (architecture_utilities::clean_run_dir(user_id) != 0){
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
  if (architecture_utilities::clean_run_dir(user_id) != 0){
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
  
  std::string input_path = architecture_utilities::get_run_dir_absolute_path(user_id) + "/input";
  std::string output_path = architecture_utilities::get_run_dir_absolute_path(user_id) + "/output";
  std::string exec_path = "main_exec";
  std::string correct_output_path = architecture_utilities::get_run_dir_absolute_path(user_id) + "/correct_output";
  std::string username = architecture_utilities::get_weak_user(user_id);

  
  if (!general_utilities::copy_file(submission_exec_path, architecture_utilities::get_run_dir_absolute_path(user_id) + "/" + exec_path, 0755)){

    LOG_ERROR_USER(user_id, "Couldn't copy submission_exec to run directory. Submission ID: " + submission_id + " " + submission_exec_path + " " + general_utilities::syscall_to_string("pwd") + general_utilities::syscall_to_string("whoami"));
    return result_enum::FAIL;
  }

  if (!general_utilities::copy_file(problem_input_path, input_path, 0644)){
    LOG_ERROR_USER(user_id, "Couldn't copy problem input to run directory. Problem ID: " + problem_id + ", Rev ID: "+ std::to_string(rev_id) + ", Test ID: " +  std::to_string(test_id) + ", Problem_input_path:" + problem_input_path + ", " + ", Destination_input_path: " + (architecture_utilities::get_run_dir_absolute_path(user_id) + "/" + input_path) + general_utilities::syscall_to_string("pwd") + general_utilities::syscall_to_string("whoami"));
    return result_enum::FAIL;
  }

  LOG_INFO_USER(user_id, "Current status: " + general_utilities::syscall_to_string("whoami") + general_utilities::syscall_to_string("ls " + architecture_utilities::get_run_dir_absolute_path(user_id)) + general_utilities::syscall_to_string("pwd"));

  auto runner_task_ptr = runner_factories::stdio_submission_runner_factory[submission.language](
    submission_id,
    exec_path,
    input_path,
    output_path,
    problem.time_limit,
    problem.memory_limit,
    0,
    false);
  if(runner_task_ptr == nullptr){
    LOG_ERROR_USER(user_id, "Failed to create runner task");
    return result_enum::FAIL;
  }

  auto runner_task = *runner_task_ptr;
  delete runner_task_ptr;


  LOG_INFO_USER(user_id, "Starting runner task execution. Submission ID: " + submission_id + ", Problem ID: " + problem_id + ", Rev ID: "+ std::to_string(rev_id) + ", Test ID: " + std::to_string(test_id) + general_utilities::syscall_to_string("ls " + architecture_utilities::get_run_dir_absolute_path(user_id)) + general_utilities::syscall_to_string("whoami"));
  helper.test_result = runner_task.execute(thread_id, user_id);
  helper.test.time_used = runner_task.get_time_consumed();
  helper.test.memory_used = runner_task.get_memory_consumed();

  if (helper.test_result == result_enum::FAIL)
  {
    LOG_ERROR_USER(user_id, "Runner task execution failed");
    return result_enum::FAIL;
  }

  if (helper.test_result != result_enum::OK){
    //std::string err_dump = general_utilities::syscall_to_string("cat " + output_path);
    //LOG_ERROR_USER(user_id, "CRASH DUMP:\n" + err_dump);
    helper.test.points = 0;
    helper.test.result = helper.test_result;
    return helper.test.result;
  }

  //correct output
  if (!general_utilities::copy_file(problem_correct_output_path, correct_output_path, 0644)){
    LOG_ERROR_USER(user_id, "Couldn't copy problem correct output to run directory. Problem ID: " + problem_id + ", Rev ID: "+ std::to_string(rev_id) + ", Test ID: " + std::to_string(test_id));
    return result_enum::FAIL;
  }
  
  //sursa concurentului
  if (!general_utilities::copy_file(architecture_utilities::get_submission_source_path(submission_id, submission.language), architecture_utilities::get_run_dir_absolute_path(user_id) + "/" + architecture_utilities::get_submission_source_name(submission.language), 0644)){
    LOG_ERROR_USER(user_id, "Couldn't copy submission source code to run directory. Submission ID: " + submission_id + ", Problem ID: " + problem_id + ", Rev ID: "+ std::to_string(rev_id) + ", Test ID: " + std::to_string(test_id));
    return result_enum::FAIL;
  }

  //checker_exec
  std::string checker_exec_name = problem.tests[test_id].checker_args[0];
  if (!general_utilities::copy_file(architecture_utilities::get_problem_checker_exec_path(problem_id, rev_id, checker_exec_name), architecture_utilities::get_run_dir_absolute_path(user_id) + "/" + checker_exec_name, 0755)){
    LOG_ERROR_USER(user_id, "Couldn't copy checker executable to run directory. Problem ID: " + problem_id + ", Rev ID: "+ std::to_string(rev_id) + ", Test ID: " + std::to_string(test_id));
    return result_enum::FAIL;
  }

  LOG_DEBUG_USER(user_id, "Files prepared for checker execution. Run directory contents:\n" + general_utilities::syscall_to_string("ls -l " + architecture_utilities::get_run_dir_absolute_path(user_id)) + "\nChecker exec path: " + architecture_utilities::get_run_dir_absolute_path(user_id) + "/" + checker_exec_name);

  auto checker_ptr = runner_factories::checker_runner_factory[submission.language](
    submission_id,
    checker_exec_name,
    architecture_utilities::get_run_dir_absolute_path(user_id) + "/checker_output_path",
    architecture_utilities::get_run_dir_absolute_path(user_id) + "/checker_message_path",
    "input",
    "output",
    "correct_output",
    architecture_utilities::get_submission_source_name(submission.language),
    problem.tests[test_id].checker_args,
    1);
  if (checker_ptr == nullptr){
    LOG_ERROR_USER(user_id, "Failed to create checker task");
    return result_enum::FAIL;
  }
  auto checker = *checker_ptr;
  delete checker_ptr;

  auto checker_result = checker.execute(thread_id, user_id);
  
  LOG_INFO_USER(user_id, "Checker task finished. Result: " + general_utilities::enum_to_string(checker_result) + ", Exit code: " + std::to_string(checker.get_exit_code()) + ", Time used: " + std::to_string(checker.get_time_consumed()) + " ms, Memory used: " + std::to_string(checker.get_memory_consumed()) + " B");

  if ((checker.get_exit_code() == 0 || checker_result != result_enum::RTE) && checker_result != result_enum::OK){
    LOG_ERROR_USER(user_id, "Checker task execution failed");
    LOG_ERROR_USER(user_id, "Checker output: " + general_utilities::syscall_to_string("cat " + architecture_utilities::get_run_dir_absolute_path(user_id) + "/" + "checker_output_path"));
    LOG_ERROR_USER(user_id, "Checker message: " + general_utilities::syscall_to_string("cat " + architecture_utilities::get_run_dir_absolute_path(user_id) + "/" + "checker_message_path"));
    return result_enum::FAIL;
  }
  

  LOG_DEBUG_USER(user_id, "Contestant output" + general_utilities::syscall_to_string("cat " + output_path));

  if (checker.get_exit_code() == 0){
    helper.test.points = 1;
    helper.test.message = "OK";
    helper.test.result = result_enum::OK;
  } else {
    helper.test.points = 0;
    helper.test.message = "Wrong answer";
    helper.test.result = result_enum::WA;
  }


  // if (checker.get_point_percentage() == 1){
  //   helper.test.points = 1;
  //   helper.test.result = result_enum::OK;
  // } else if (checker.get_point_percentage() > 0){
  //   helper.test.points = 0.5;
  //   helper.test.result = result_enum::PA;
  // } else {
  //   helper.test.points = 0;
  //   helper.test.result = result_enum::WA;
  // }
  
  // helper.test.message = checker.get_message();
  
  return helper.test.result;
}
