#include <stdio_runner_factory.h>

#include <cstdio>
#include <cstdlib>
#include <string>

#include <stdio_runner_task.h>

std::unique_ptr<task> stdio_runner_factory(
    language_enum language,
    long submission_id,
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

    if (submission_id <= 0)
    {
        return nullptr;
    }

    const char *sandbox_path = getenv("SANDBOX_PATH");
    if (sandbox_path == nullptr || sandbox_path[0] == '\0')
    {
        return nullptr;
    }

    const std::string exec_path =
        std::string(sandbox_path) + "/submissions/" + std::to_string(submission_id) + "/main_exec";

    std::unique_ptr<task> created = std::make_unique<stdio_runner_task>(
        exec_path,
        input_path,
        output_path,
        run_time_limit,
        run_memory_limit);

    created->priority = priority;
    return created;
}
