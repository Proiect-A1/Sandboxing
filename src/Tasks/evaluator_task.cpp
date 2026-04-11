#include <Tasks/evaluator_task.h> 
result_enum evaluator_task::execute(pthread_t thread_id, int user_id) {
  if (user_id <= 0) {
    print_error(thread_id, user_id, "Invalid user ID");
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
    print_error(thread_id, user_id, "Failed to create compiler task");
    return result_enum::FAIL;
  }
  result = compiler->execute(thread_id, user_id);
  if (result != result_enum::OK) {
    print_log(thread_id, user_id, "Compilation failed on submission " + submission_id + " with result " + general_utilities::enum_to_string(result));
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

  print_log(thread_id, user_id, "Evaluation tasks for submission " + submission_id + " have been created and added to the queue");
  return result_enum::OK;

}

void evaluator_task:: print_log(pthread_t thread_id, int user_id,const std::string& message){
  fprintf(stdout, "\033[93m[LOG  ]\033[0m Evaluator task running on thread %lu, with user %d: %s\n", thread_id, user_id, message.c_str());
}

void evaluator_task:: print_error(pthread_t thread_id, int user_id,const std::string& message){
  fprintf(stderr, "\033[31m[ERROR]\033[0m Evaluator task running on thread %lu, with user %d: %s\n", thread_id, user_id, message.c_str());
}