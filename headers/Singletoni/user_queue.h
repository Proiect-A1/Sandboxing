#ifndef USER_QUEUE_H
#define USER_QUEUE_H

#include <Utilities/mutex_queue.h>
#include <string>
#include <pthread.h>
class user_queue{
  static user_queue* instance;
  static pthread_mutex_t mtx;
  mutex_queue<int> queue;
  user_queue(){}
  public:
  static user_queue& get_instance();
  bool empty();
  int size();
  void push(const int& item);
  int pop();
  int front();
};


#endif // USER_QUEUE_H