#include <Singletoni/memory_manager.h>
#include <Singletoni/logger.h>


memory_manager* memory_manager::instance = nullptr;
pthread_mutex_t memory_manager::mtx = PTHREAD_MUTEX_INITIALIZER;

memory_manager& memory_manager::get_instance() {
  if (instance == nullptr) {
    pthread_mutex_lock(&mtx);
    if (instance == nullptr)
    instance = new memory_manager();
    pthread_mutex_unlock(&mtx);
  }
	return *instance;
}

bool memory_manager::request_memory(unsigned long long size) {
	pthread_mutex_lock(&mtx);
	if (memory_used + size > memory_limit) {
		LOG_WARNING(std::string("Memory request denied (requested: ") + std::to_string(size) + ", used: " + std::to_string(memory_used) + ", limit: " + std::to_string(memory_limit) + ")");
		pthread_mutex_unlock(&mtx);
		return false;
	}
	memory_used += size;
	LOG_INFO(std::string("Memory request granted (requested: ") + std::to_string(size) + ", used: " + std::to_string(memory_used) + ")");
	pthread_mutex_unlock(&mtx);
	return true;
}

void memory_manager::blocking_request_memory(unsigned long long size) {
	pthread_mutex_lock(&mtx);
	if (memory_used + size > memory_limit) {
		LOG_INFO(std::string("Waiting for memory (requested: ") + std::to_string(size) + ", used: " + std::to_string(memory_used) + ", limit: " + std::to_string(memory_limit) + ")");
	}
	while (memory_used + size > memory_limit) {
		pthread_cond_wait(&cond, &mtx);
	}
	memory_used += size;
	LOG_INFO(std::string("Memory granted after wait (requested: ") + std::to_string(size) + ", used: " + std::to_string(memory_used) + ")");
	pthread_mutex_unlock(&mtx);
}

void memory_manager::release_memory(unsigned long long size) {
	pthread_mutex_lock(&mtx);
	memory_used -= size;
	LOG_INFO(std::string("Memory released (amount: ") + std::to_string(size) + ", used: " + std::to_string(memory_used) +")");
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mtx);
}
