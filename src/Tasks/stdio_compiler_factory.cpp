#include <Tasks/stdio_compiler_factory.h>

stdio_compiler_task* stdio_compiler_factory(
    language_enum language,
    std::string submission_id,
    uint8_t priority)
{

    if (language != language_enum::CPP && language != language_enum::RUST && language != language_enum::C && language != language_enum::CSHARP && language != language_enum::GO && language != language_enum::PYTHON)
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
            {"/usr/bin/rustc", "--edition=2021", "-O", "-C", "linker=/usr/bin/gcc", "-o", "main_exec", "main.rs"}
        );
    } else if (language == language_enum::C) {
        created = new stdio_compiler_task(
            "/usr/bin/gcc",
            "main.c",
            "main_exec",
            submission_id,
            language,
            compilation_limits::COMPILATION_TIME_LIMIT.at(language),
            compilation_limits::EXECUTABLE_SIZE_LIMIT.at(language),
            {"/usr/bin/gcc", "-O2", "-o", "main_exec", "main.c", "-lm"}
        );
    } else if (language == language_enum::CSHARP) {
        created = new stdio_compiler_task(
            "/bin/bash",
            "main.cs",
            "main_exec",
            submission_id,
            language,
            compilation_limits::COMPILATION_TIME_LIMIT.at(language),
            compilation_limits::EXECUTABLE_SIZE_LIMIT.at(language),
            {"/bin/bash", "-c", "dotnet new console -n App -o . --force && mv main.cs Program.cs && dotnet publish -c Release -r linux-x64 -p:PublishAot=true -p:OptimizationPreference=Speed -p:StripSymbols=true -p:InvariantGlobalization=true -o out_dir && mv out_dir/App main_exec"}
        );
    } else if (language == language_enum::GO) {
        created = new stdio_compiler_task(
            "/usr/bin/go",
            "main.go",
            "main_exec",
            submission_id,
            language,
            compilation_limits::COMPILATION_TIME_LIMIT.at(language),
            compilation_limits::EXECUTABLE_SIZE_LIMIT.at(language),
            {"/usr/bin/go", "build", "-o", "main_exec", "main.go"}
        );
    } else if (language == language_enum::PYTHON) {
        created = new stdio_compiler_task(
            "/bin/bash",
            "main.py",
            "main_exec",
            submission_id,
            language,
            compilation_limits::COMPILATION_TIME_LIMIT.at(language),
            compilation_limits::EXECUTABLE_SIZE_LIMIT.at(language),
            {"/bin/bash", "-c", "/usr/bin/python3 -m py_compile main.py && echo '#!/usr/bin/python3 -I' > main_exec && cat main.py >> main_exec && chmod +x main_exec"}
        );
    }

    created->priority = priority;
    return created;
}
