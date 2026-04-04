#ifndef TASK_H
#define TASK_H

#include <cstdint>
#include <result_enum.h>

struct task
{
  uint8_t priority = 0;
  virtual ~task() = default;
  virtual bool check_permissions() = 0;
  virtual result_enum execute(int thread_id, int user_id) = 0;
};

#endif