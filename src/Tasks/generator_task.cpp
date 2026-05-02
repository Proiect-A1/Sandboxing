#include <Tasks/generator_task.hpp>

result_enum generator_task::execute(pthread_t thread_id, int user_id) {
  LOG_INFO_USER(user_id, "Executing generator task for problem " + std::to_string(problem_id) + " revision " + std::to_string(rev_id));

  //se compileaza scriptul

  auto tq = task_queue::get_instance();

  for (int i = 0; i < test_infos.size(); i++) {
    tq.push(new single_test_generator_task(test_infos[i], i, problem_id, rev_id));
  }

  // a result of OK means just that all single_test_generators were succesfully launched, if one of them fails after then that will be handled separately from this task
  return result_enum::OK;
}