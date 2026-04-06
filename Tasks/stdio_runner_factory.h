#ifndef STDIO_RUNNER_FACTORY_H
#define STDIO_RUNNER_FACTORY_H

#include <cstdint>
#include <memory>
#include <string>

#include <Enums/language_enum.h>
#include <Tasks/task.h>
#include <Utilities/submission_info_utilities.h>

std::unique_ptr<task> stdio_runner_factory(
    language_enum language,
    std::string submission_id,
    std::string exec_path,
    std::string input_path,
    std::string output_path,
    float run_time_limit,
    long run_memory_limit,
    uint8_t priority = 0);

#endif
