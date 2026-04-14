#include <Singletoni/user_queue.h>

user_queue* user_queue::instance = nullptr;
pthread_mutex_t user_queue::mtx = PTHREAD_MUTEX_INITIALIZER;

user_queue& user_queue::get_instance(){
  if (instance == nullptr) {
      pthread_mutex_lock(&user_queue::mtx);
      if (instance == nullptr)
        instance = new user_queue();
      pthread_mutex_unlock(&user_queue::mtx);
  }
  return *instance;
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
