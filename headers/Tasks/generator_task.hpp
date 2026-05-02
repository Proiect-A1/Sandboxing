#ifndef GENERATOR_TASK_HPP
#define GENERATOR_TASK_HPP
#include <string>
#include <Chestii_cu_data/problem_metadata.h>
#include <Chestii_cu_data/test_generation_info.hpp>
#include <Tasks/task.h>
#include <Tasks/single_test_generator_task.hpp>
#include <Singletoni/task_queue.h>
class generator_task : public task{
    int problem_id;
    int rev_id;
    std::string problem_folder_root_path;
    std::string script_path;
    std::vector<test_generation_info> test_infos;
public:
    generator_task(int problem_id, int rev_id, std::string problem_folder_root_path);
    std::vector<std::string> compile(std::string script_path);
    result_enum execute(pthread_t thread_id, int user_id);
};
#endif // GENERATOR_TASK_HPP