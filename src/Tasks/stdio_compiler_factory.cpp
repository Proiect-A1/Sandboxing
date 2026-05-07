#include <Tasks/stdio_compiler_factory.h>

stdio_compiler_task* stdio_compiler_factory(
    language_enum language,
    std::string submission_id,
    uint8_t priority)
{

    if (language != language_enum::CPP && language != language_enum::RUST)
    {
        return nullptr;
    }

    stdio_compiler_task* created = nullptr;

    if (language == language_enum::CPP) {
        created = new stdio_compiler_task(
            "/usr/bin/g++",
            "main.cpp",
            "main_exec",
            submission_id,
            language,
            compilation_limits::COMPILATION_TIME_LIMIT.at(language),
            compilation_limits::EXECUTABLE_SIZE_LIMIT.at(language),
            {"/usr/bin/g++", "-std=c++17", "-O2", "-o", "main_exec", "main.cpp"}
        );
    } else if (language == language_enum::RUST) {
        created = new stdio_compiler_task(
            "/usr/bin/rustc",
            "main.rs",
            "main_exec",
            submission_id,
            language,
            compilation_limits::COMPILATION_TIME_LIMIT.at(language),
            compilation_limits::EXECUTABLE_SIZE_LIMIT.at(language),
            {"/usr/bin/rustc", "--edition=2021", "-O", "-o", "main_exec", "main.rs"}
        );
    }

    created->priority = priority;
    return created;
}
