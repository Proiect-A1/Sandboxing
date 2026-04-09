#include <Tasks/evaluator_task.h> 
result_enum evaluator_task::execute(int thread_id, int user_id) {
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
    print_error(thread_id, user_id, "Compilation failed");
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
  return result_enum::OK;

}

void evaluator_task:: print_error(int thread_id, int user_id,const std::string& message){
  fprintf(stderr, "Evaluator task running on thread %d, with user %d: %s\n", thread_id, user_id, message.c_str());
}