#include <Singletoni/core_queue.h>

pthread_mutex_t core_queue::mtx = PTHREAD_MUTEX_INITIALIZER;

core_queue& core_queue::get_instance(){
  static core_queue instance;
  return instance;
}

bool core_queue::empty(){
  return queue.empty();
}

int core_queue::size(){
  return queue.size();
}

void core_queue::push(const int& item){
  queue.push(item);
}

int core_queue::pop(){
  return queue.pop();
}

int core_queue::front(){
  return queue.front();
}
