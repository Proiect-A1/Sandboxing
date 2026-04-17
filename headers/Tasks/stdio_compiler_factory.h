#ifndef STDIO_COMPILER_FACTORY_H
#define STDIO_COMPILER_FACTORY_H

#include <cstdint>
#include <memory>

#include <Enums/language_enum.h>
#include <Tasks/task.h>
#include <Tasks/stdio_compiler_task.h>

#include <Utilities/compilation_limits.h>

stdio_compiler_task* stdio_compiler_factory(
    language_enum language,
    std::string submission_id,
    uint8_t priority = 0);

#endif
