#include <Singletoni/submission_manager.h>
#include <Singletoni/logger.h>

submission_manager* submission_manager::instance = nullptr;
pthread_mutex_t submission_manager::mtx = PTHREAD_MUTEX_INITIALIZER;

submission_manager& submission_manager::get_instance(){
    if (instance == nullptr) {
        pthread_mutex_lock(&submission_manager::mtx);
        if (instance == nullptr)
          instance = new submission_manager();
        pthread_mutex_unlock(&submission_manager::mtx);
    }
    return *instance;
  }

submission_data submission_manager::get_submission(std::string submission_id){
    pthread_mutex_lock(&submission_manager::mtx);
    submission_data retval;
    if(submission_table.count(submission_id)){
        retval=submission_table[submission_id];
    } 
    else {
        LOG_WARNING(std::string("Submission not found for id ") + submission_id);
    }
    pthread_mutex_unlock(&submission_manager::mtx);
    return retval;
}
std::map<std::string, submission_data> submission_manager::get_submission_table(){
    pthread_mutex_lock(&mtx);
    std::map<std::string, submission_data> retval=this->submission_table;
    pthread_mutex_unlock(&mtx);
    return retval;
}
void submission_manager::insert(std::string submission_id, language_enum language, std::string problem_id, int rev_id, int socket_fd){
    submission_data sd(language, problem_id, rev_id, socket_fd);
    pthread_mutex_lock(&submission_manager::mtx);
    submission_table[submission_id]=sd;
    LOG_INFO(std::string("Inserted submission ") + submission_id + " for problem " + problem_id + " rev " + std::to_string(rev_id));
    pthread_mutex_unlock(&submission_manager::mtx);
}

void submission_manager::erase(std::string submission_id){
    pthread_mutex_lock(&submission_manager::mtx);
    const size_t erased_count = submission_table.erase(submission_id);
    if (erased_count == 0) {
        LOG_WARNING(std::string("Attempted to erase missing submission ") + submission_id);
    } else {
        LOG_INFO(std::string("Erased submission ") + submission_id);
    }
    pthread_mutex_unlock(&submission_manager::mtx);
}

bool submission_manager::count(std::string submission_id){
    pthread_mutex_lock(&submission_manager::mtx);
    bool retval=submission_table.count(submission_id);
    pthread_mutex_unlock(&submission_manager::mtx);
    return retval;
}
bool submission_manager::is_done(std::string submission_id){
    return !count(submission_id); /// TODO: de distins dintre surse terminate si surse inexistente
    
}
void submission_manager::add_completed_test(std::string submission_id, int test_id, submission_test test_result){
    pthread_mutex_lock(&submission_manager::mtx);
    if (!submission_table.count(submission_id)) {
        LOG_ERROR(std::string("Cannot add completed test for missing submission ") + submission_id);
        pthread_mutex_unlock(&submission_manager::mtx);
        return;
    }
    // std::cout << general_utilities::enum_to_string(test_result.result) << ' ' << test_result.points << std::endl;
    submission_table[submission_id].add_completed_test(test_id, test_result);
    LOG_INFO(std::string("Added completed test ") + std::to_string(test_id) + " to submission " + submission_id);
    pthread_mutex_unlock(&submission_manager::mtx);
}