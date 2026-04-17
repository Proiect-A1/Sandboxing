#ifndef TASK_H
#define TASK_H

#include <cstdint>
#include <string>
#include <Enums/result_enum.h>

struct task
{
  uint8_t priority = 0;
  virtual ~task() = default;
  virtual bool check_permissions() = 0;
  virtual result_enum execute(pthread_t thread_id, int user_id) = 0;
  virtual void print_error(pthread_t thread_id, int user_id,const std::string& message) = 0;
  virtual void print_log(pthread_t thread_id, int user_id,const std::string& message) = 0;
  bool operator<(const task& other) const {
    return priority < other.priority;
  }
};

#endif