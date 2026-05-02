#include <Singletoni/user_queue.h>

pthread_mutex_t user_queue::mtx = PTHREAD_MUTEX_INITIALIZER;

user_queue& user_queue::get_instance(){
  static user_queue instance;
  return instance;
}

bool user_queue::empty(){
  return queue.empty();
}

int user_queue::size(){
  return queue.size();
}

void user_queue::push(const int& item){
  queue.push(item);
}

int user_queue::pop(){
  return queue.pop();
}

int user_queue::front(){
  return queue.front();
}
