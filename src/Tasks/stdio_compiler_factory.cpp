#include <Tasks/stdio_compiler_factory.h>

std::unique_ptr<task> stdio_compiler_factory(
    language_enum language,
    std::string submission_id,
    uint8_t priority)
{

    if (language != language_enum::CPP)
    {
        return nullptr;
    }


    std::unique_ptr<task> created = std::make_unique<stdio_compiler_task>(
        "/usr/bin/g++",
        "main.cpp",
        "main_exec",
        submission_id,
            compilation_limits::COMPILATION_TIME_LIMIT.at(language),
            compilation_limits::EXECUTABLE_SIZE_LIMIT.at(language)
        );

    created->priority = priority;
    return created;
}
