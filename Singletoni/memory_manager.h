#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <pthread.h>
class memory_manager {
  //Singleton
  static memory_manager* instance;

  static pthread_mutex_t mtx;
  unsigned long long memory_used = 0; // in bytes
  unsigned long long memory_limit = 1024ll * 1024 * 1024 * 15; // in bytes, default 1GB
  memory_manager() = default;
  public:
  static memory_manager& get_instance(){
    if (instance == nullptr) {
      pthread_mutex_lock(&mtx);
      instance = new memory_manager();
      pthread_mutex_unlock(&mtx);
    }
    return *instance;
  }
};



#endif // MEMORY_MANAGER_H