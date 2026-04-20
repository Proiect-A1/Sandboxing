#ifndef CORE_QUEUE_H
#define CORE_QUEUE_H

#include <Utilities/mutex_queue.h>
#include <string>
#include <pthread.h>
class core_queue{
  static pthread_mutex_t mtx;
  mutex_queue<int> queue;
  core_queue(){}
  public:
  static core_queue& get_instance();
  bool empty();
  int size();
  void push(const int& item);
  int pop();
  int front();
};


#endif // CORE_QUEUE_H