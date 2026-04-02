#ifndef TASK_H
#define TASK_H

struct task{
  __UINT8_TYPE__ priority;
  virtual bool check_permissions() = 0;
  virtual bool execute(int thread_id, int user_id) = 0;
};

#endif