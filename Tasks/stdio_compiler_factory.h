#ifndef STDIO_COMPILER_FACTORY_H
#define STDIO_COMPILER_FACTORY_H

#include <cstdint>
#include <memory>

#include <language_enum.h>
#include <task.h>

std::unique_ptr<task> stdio_compiler_factory(
    language_enum language,
    long submission_id,
    float compile_time_limit,
    long exec_size_limit,
    uint8_t priority = 0);

#endif
