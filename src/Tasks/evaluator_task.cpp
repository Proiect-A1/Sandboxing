#include <Tasks/evaluator_task.h> 
result_enum evaluator_task::execute(pthread_t thread_id, int user_id) {
  if (user_id <= 0) {
    LOG_ERROR_USER(user_id, "Invalid user ID");
    return result_enum::FAIL;
  }
  result_enum result;
  problem_manager& pm = problem_manager::get_instance();
  auto compiler = stdio_compiler_factory(
    language_enum::CPP,
    submission_id,
    0 // priority
  );
  if (!compiler) {
    LOG_ERROR_USER(user_id, "Failed to create compiler task");
    return result_enum::FAIL;
  }
  result = compiler->execute(thread_id, user_id);
  if (result != result_enum::OK) {
    LOG_INFO_USER(user_id, "Compilation failed on submission " + submission_id + " with result " + general_utilities::enum_to_string(result));
    // trebuie bagat in submission manager
    return result;
  }




  problem_metadata problem = pm.get_metadata(problem_id, rev_id);
  for (int test_id = 0; test_id < problem.test_count; test_id++) {
      task_queue::get_instance().push(new stdio_grader_task(
      submission_id,
      problem_id,
      rev_id,
      test_id
    ));
  }

  LOG_INFO_USER(user_id, "Evaluation tasks for submission " + submission_id + " have been created and added to the queue");
  return result_enum::OK;

}
