#include <Singletoni/task_queue.h>

task_queue* task_queue::instance = nullptr;
pthread_mutex_t task_queue::mtx = PTHREAD_MUTEX_INITIALIZER;

task_queue& task_queue::get_instance(){
  if (instance == nullptr) {
      pthread_mutex_lock(&task_queue::mtx);
      if (instance == nullptr)
        instance = new task_queue();
      pthread_mutex_unlock(&task_queue::mtx);
  }
  return *instance;
}

bool task_queue::empty(){
  return queue.empty();
}
int task_queue::size(){
  return queue.size();
}

void task_queue::push(task* item){
  queue.push(item);
}

task* task_queue::pop(){
  return queue.pop();
}

task* task_queue::top(){
  return queue.top();
}