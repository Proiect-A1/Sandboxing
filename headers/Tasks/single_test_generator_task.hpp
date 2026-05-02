#ifndef SINGLE_TEST_GENERATOR_TASK_HPP
#define SINGLE_TEST_GENERATOR_TASK_HPP
#include <Chestii_cu_data/test_generation_info.hpp>
#include <Tasks/task.h>

class single_test_generator_task : public task{
  test_generation_info info;
  int problem_id;
  int rev_id;
  int test_id;
public:
    single_test_generator_task(test_generation_info info, int test_id, int problem_id, int rev_id) : info(info), test_id(test_id), problem_id(problem_id), rev_id(rev_id) {}
    result_enum execute(pthread_t thread_id, int user_id) override;
    bool check_permissions() override { return true; };
};


#endif // SINGLE_TEST_GENERATOR_TASK_HPP