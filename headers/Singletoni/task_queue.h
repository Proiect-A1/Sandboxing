#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#include <Utilities/mutex_priority_queue.h>
#include <Utilities/general_utilities.h>
#include <Tasks/task.h>
#include <string>
#include <pthread.h>
class task_queue{
  static task_queue* instance;
  static pthread_mutex_t mtx;
  task_queue(){}
  mutex_priority_queue<task*, general_utilities::pointer_compare<task>> queue;
  public:
  static task_queue& get_instance();
  bool empty();
  int size();
  void push(task* item);
  task* pop();
  task* top();
};

#endif  // TASK_QUEUE_H