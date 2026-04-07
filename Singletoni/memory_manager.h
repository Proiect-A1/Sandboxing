#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <pthread.h>
#include <string.h>
class memory_manager {
  //Singleton
  static memory_manager* instance;

  static pthread_mutex_t mtx;
  
  unsigned long long memory_used = 0; // in bytes
  unsigned long long memory_limit = 1024ll * 1024 * 1024 * 15; // in bytes, default 15GB
  pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
  memory_manager() = default;
  public:
  static memory_manager& get_instance();
  bool request_memory(unsigned long long size);
  void blocking_request_memory(unsigned long long size);
  void release_memory(unsigned long long size);
};



#endif // MEMORY_MANAGER_H