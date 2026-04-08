#ifndef STDIO_COMPILER_TASK_H
#define STDIO_COMPILER_TASK_H

#include <string>

#include <Tasks/task.h>
#include <Utilities/utilities.h>
#include <Utilities/submission_info_utilities.h>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <filesystem>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>

class stdio_compiler_task : public task
{
    std::string compile_command;

    std::string submission_id;
    float time_limit;     // in milliseconds
    long exec_size_limit; // in bytes

public:
    stdio_compiler_task(
        std::string compile_command,
        std::string source_file_name,
        std::string output_file_name,
        std::string submission_id,
        float time_limit,
        long exec_size_limit)
        : compile_command(compile_command),
          submission_id(submission_id),
          time_limit(time_limit),
          exec_size_limit(exec_size_limit)
    {
    }

    bool check_permissions() override;
    result_enum execute(int thread_id, int user_id) override;
};

#endif
