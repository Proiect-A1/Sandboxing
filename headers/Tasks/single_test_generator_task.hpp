// genereaza un test :D
#ifndef SINGLE_TEST_GENERATOR_TASK_H
#define SINGLE_TEST_GENERATOR_TASK_H
#include <string>

#include <Chestii_cu_data/problem_metadata.h>
#include <Chestii_cu_data/test_metadata.h>
#include <Tasks/task.h>
#include <Tasks/runner_factories.hpp>
#include <Tasks/checker_task.h>
#include <Singletoni/logger.h>
#include <Utilities/architecture_utilities.h>
class single_test_generator_task : public task {
    struct stgt_helper {
      std::string problem_id;
      int rev_id;
      int test_id;
      result_enum result;
      int user_id;
      stgt_helper(const std::string& problem_id, int rev_id, int test_id) : problem_id(problem_id), rev_id(rev_id), test_id(test_id), result(result_enum::OK) {}
      ~stgt_helper();
    };
    std::string problem_id;
    int rev_id;
    int test_id;
    test_metadata test_meta;
    long long memory_limit;
    float time_limit; // in ms
public:
    single_test_generator_task(const std::string& problem_id, int rev_id, int test_id, const test_metadata& test_meta, long long memory_limit, float time_limit) : problem_id(problem_id), rev_id(rev_id), test_id(test_id), test_meta(test_meta), memory_limit(memory_limit), time_limit(time_limit) {
        priority = 1; // Prioritate mai mare pentru generarea testelor
    }
    bool check_permissions(int user_id) override {
        // Aici ar trebui să verifici dacă user_id are permisiunea de a genera
        return true;
    }
    result_enum execute(pthread_t thread_id, int user_id) override;
};

#endif