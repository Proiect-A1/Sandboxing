// genereaza un test :D
#ifndef SINGLE_TEST_GENERATOR_TASK_H
#define SINGLE_TEST_GENERATOR_TASK_H
#include <string>
#include <Chestii_cu_data/problem_metadata.h>
#include <Tasks/task.h>
#include <Chestii_cu_data/test_metadata.h>
#include <Tasks/runner_factories.hpp>
class single_test_generator_task : public task {
    std::string problem_id;
    int rev_id;
    int test_id;
    test_metadata test_meta;
public:
    single_test_generator_task(const std::string& problem_id, int rev_id, int test_id, const test_metadata& test_meta) : problem_id(problem_id), rev_id(rev_id), test_id(test_id), test_meta(test_meta) {
        priority = 1; // Prioritate mai mare pentru generarea testelor
    }
    bool check_permissions(int user_id) override {
        // Aici ar trebui să verifici dacă user_id are permisiunea de a genera
        return true;
    }
    result_enum execute(pthread_t thread_id, int user_id) override;
};

#endif