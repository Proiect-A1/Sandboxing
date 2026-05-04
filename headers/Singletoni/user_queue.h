#ifndef USER_QUEUE_H
#define USER_QUEUE_H

#include <Utilities/mutex_queue.h>
#include <string>
#include <pthread.h>
class user_queue{
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