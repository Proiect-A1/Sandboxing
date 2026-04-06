#include <Tasks/stdio_compiler_factory.h>

#include <Tasks/stdio_compiler_task.h>

std::unique_ptr<task> stdio_compiler_factory(
    language_enum language,
    std::string submission_id,
    float compile_time_limit,
    long exec_size_limit,
    uint8_t priority)
{
    (void)compile_time_limit;

    if (language != language_enum::CPP)
    {
        return nullptr;
    }

    const float very_large_compile_time_limit = 24.0f * 60.0f * 60.0f * 1000.0f;

    std::unique_ptr<task> created = std::make_unique<stdio_compiler_task>(
        "/usr/bin/g++",
        "main.cpp",
        "main_exec",
        submission_id,
        very_large_compile_time_limit,
        exec_size_limit);

    created->priority = priority;
    return created;
}
