#ifndef PENDING_SUBMISSIONS_MANAGER_H
#define PENDING_SUBMISSIONS_MANAGER_H

#include <Chestii_cu_data/submission_data.h>
#include <map>
#include <Singletoni/logger.h>
#include <pthread.h>
#include <Singletoni/submission_manager.h>
#include <Singletoni/task_queue.h>
#include <Tasks/evaluator_task.h>

class pending_submissions_manager {
  //Singleton
  static pthread_mutex_t mtx;
  std::map<std::string, std::map<int, std::queue<std::string>>> pending_submissions; // problem_id -> rev_id -> queue<submission_id>
  pending_submissions_manager() = default;
  public:
  static pending_submissions_manager& get_instance();
  void push(std::string problem_id, int rev_id, std::string submission_id);
  void problem_failed(std::string problem_id, int rev_id);
  void problem_done(std::string problem_id, int rev_id);
  int count_pending_submissions(std::string problem_id, int rev_id);
};

#endif