#include <Tasks/stdio_runner_factory.h>


stdio_runner_task* stdio_runner_factory(
    language_enum language,
    std::string submission_id,
    std::string exec_path,
    std::string input_path,
    std::string output_path,
    float run_time_limit,
    long run_memory_limit,
    uint8_t priority)
{
    if (language != language_enum::CPP)
    {
        return nullptr;
    }

    if (submission_id.empty() || exec_path.empty() || input_path.empty() || output_path.empty())
    {
        return nullptr;
    }

    stdio_runner_task* created = new stdio_runner_task(
        exec_path,
        input_path,
        output_path,
        run_time_limit,
        run_memory_limit);

    created->priority = priority;
    return created;
}
