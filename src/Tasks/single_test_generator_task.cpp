#include <Tasks/single_test_generator_task.hpp>

result_enum single_test_generator_task::execute(pthread_t thread_id, int user_id){
  (void)thread_id;
  LOG_INFO_USER(user_id, "Executing single test generator task for test " + std::to_string(test_id) + " of problem " + std::to_string(problem_id) + " revision " + std::to_string(rev_id));

  // se ruleaza generatorul


  LOG_INFO_USER(user_id, "Test generation script executed successfully for test " + std::to_string(test_id));
  return result_enum::OK;
}