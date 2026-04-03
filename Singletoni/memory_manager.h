#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <pthread.h>
class memory_manager {
  //Singleton
  static memory_manager* instance;

  static pthread_mutex_t mtx;
  
  unsigned long long memory_used = 0; // in bytes
  unsigned long long memory_limit = 1024ll * 1024 * 1024 * 15; // in bytes, default 1GB
  pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
  memory_manager() = default;
  public:
  static memory_manager& get_instance(){
    if (instance == nullptr) {
      mtx = PTHREAD_MUTEX_INITIALIZER;
      pthread_mutex_lock(&mtx);
      instance = new memory_manager();
      pthread_mutex_unlock(&mtx);
    }
    return *instance;
  }
  bool request_memory(unsigned long long size) {
    pthread_mutex_lock(&mtx);
    if (memory_used + size > memory_limit) {
      pthread_mutex_unlock(&mtx);
      return false;
    }
    memory_used += size;
    pthread_mutex_unlock(&mtx);
    return true;
  }
  void blocking_request_memory(unsigned long long size) {
    pthread_mutex_lock(&mtx);
    while (memory_used + size > memory_limit) {
      pthread_cond_wait(&cond, &mtx);
    }
    memory_used += size;
    pthread_mutex_unlock(&mtx);
  }
  void release_memory(unsigned long long size){
    pthread_mutex_lock(&mtx);
    memory_used -= size;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mtx);
  }
};



#endif // MEMORY_MANAGER_H