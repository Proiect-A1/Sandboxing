#include <Singletoni/memory_manager.h>


memory_manager* memory_manager::instance = nullptr;
pthread_mutex_t memory_manager::mtx = PTHREAD_MUTEX_INITIALIZER;

memory_manager& memory_manager::get_instance() {
	pthread_mutex_lock(&mtx);
  if (instance == nullptr) {
    instance = new memory_manager();
    void *ptr = mmap(NULL, sizeof(memory_manager), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    memcpy(ptr, instance, sizeof(memory_manager));
    instance = (memory_manager*)ptr;
  }
	pthread_mutex_unlock(&mtx);
	return *instance;
}

bool memory_manager::request_memory(unsigned long long size) {
	pthread_mutex_lock(&mtx);
	if (memory_used + size > memory_limit) {
		pthread_mutex_unlock(&mtx);
		return false;
	}
	memory_used += size;
	pthread_mutex_unlock(&mtx);
	return true;
}

void memory_manager::blocking_request_memory(unsigned long long size) {
	pthread_mutex_lock(&mtx);
	while (memory_used + size > memory_limit) {
		pthread_cond_wait(&cond, &mtx);
	}
	memory_used += size;
	pthread_mutex_unlock(&mtx);
}

void memory_manager::release_memory(unsigned long long size) {
	pthread_mutex_lock(&mtx);
	memory_used -= size;
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mtx);
}
