#include <Singletoni/submission_manager.h>
#include <Chestii_cu_data/submission_data.h>

submission_manager* submission_manager::instance = nullptr;
pthread_mutex_t submission_manager::lock = PTHREAD_MUTEX_INITIALIZER;

submission_manager& submission_manager::get_instance(){
    if (instance == nullptr) {
        pthread_mutex_lock(&lock);
        instance = new submission_manager();
        void *ptr = mmap(NULL, sizeof(submission_manager), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
        memcpy(ptr, instance, sizeof(submission_manager));
        instance = (submission_manager*)ptr;
        pthread_mutex_unlock(&lock);
    }
    return *instance;
  }

void submission_manager::insert(std::string submission_id, std::string problem_id, int rev_id){
    // submission_data sd(problem_id, rev_id);
    pthread_mutex_lock(&lock);
    // submission_table[submission_id]=sd;
    pthread_mutex_unlock(&lock);
}

void submission_manager::erase(std::string submission_id){
    pthread_mutex_lock(&lock);
    submission_table.erase(submission_id);
    pthread_mutex_unlock(&lock);
}

bool submission_manager::count(std::string submission_id){
    pthread_mutex_lock(&lock);
    bool ans = submission_table.count(submission_id) > 0;
    pthread_mutex_unlock(&lock);
    return ans;
}
bool submission_manager::is_done(std::string submission_id){
    return !count(submission_id); /// TODO: de distins dintre surse terminate si surse inexistente
    
}
void submission_manager::set_test_result(std::string submission_id, int test_id, submission_test test_result){
    // TODO
}
submission_data submission_manager::get_submission(std::string submission_id){
    pthread_mutex_lock(&lock);
    submission_data ans = submission_table[submission_id];
    pthread_mutex_unlock(&lock);
    return ans;
}