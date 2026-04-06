#include <submission_manager.h>
#include <Chestii_cu_data/submission_data.h>
submission_manager& submission_manager::get_instance(){
    if (instance == nullptr) {
        lock = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_lock(&lock);
        instance = new submission_manager();
        pthread_mutex_unlock(&lock);
    }
    return *instance;
  }

void submission_manager::insert(long submission_id, int problem_id, int rev_id){
    submission_data sd(problem_id, rev_id);
    pthread_mutex_lock(&lock);
    submission_table[submission_id]=sd;
    pthread_mutex_unlock(&lock);
}

void submission_manager::erase(long submission_id){
    pthread_mutex_lock(&lock);
    submission_table.erase(submission_id);
    pthread_mutex_unlock(&lock);
}

bool submission_manager::count(long submission_id){
    pthread_mutex_lock(&lock);
    submission_table.count(submission_id);
    pthread_mutex_unlock(&lock);
}
bool submission_manager::is_done(long submission_id){
    return !count(submission_id); /// TODO: de distins dintre surse terminate si surse inexistente
    
}
void submission_manager::set_test_result(long submission_id, int test_id, submission_test test_result){
    // TODO
}