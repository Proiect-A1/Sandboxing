#include <Tasks/single_test_generator_task.hpp>

result_enum single_test_generator_task::execute(pthread_t thread_id, int user_id){

  auto generator_task = runner_factories::generator_runner_factory[language_enum::COMPILED](
    problem_id + "_rev" + std::to_string(rev_id) + "_test" + std::to_string(test_id),
    test_meta.generator_args[0],
    "output",
    "message",
    test_meta.generator_args,
    1 // priority
  );
  return result_enum::OK;
}