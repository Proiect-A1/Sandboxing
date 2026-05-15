// genereaza testele la o problema
#ifndef GENERATOR_TASK_H
#define GENERATOR_TASK_H
#include <string>
#include <Chestii_cu_data/problem_metadata.h>
#include <Chestii_cu_data/submission_test.h>
#include <Singletoni/logger.h>
#include <Singletoni/problem_manager.h>
#include <Singletoni/submission_manager.h>
#include <Singletoni/task_queue.h>
#include <Tasks/task.h>
#include <Tasks/super_runner_task.hpp>
#include <Tasks/runner_factories.hpp>
#include <Tasks/checker_task.h>
#include <Tasks/single_test_generator_task.hpp>
#include <Tasks/test_generation_script_compiler_task.hpp>
#include <Utilities/architecture_utilities.h>
#include <Utilities/general_utilities.h>
#include <string>
class generator_task : public task{
    std::string problem_id;
    int rev_id;
public:
    generator_task(const std::string& problem_id, int rev_id) : problem_id(problem_id), rev_id(rev_id) {
        priority = 2;
    }
    result_enum execute(pthread_t thread_id, int user_id) override;
    bool check_permissions(int user_id) override {return true;};
};
#endif