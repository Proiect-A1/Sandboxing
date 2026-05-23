#include <Singletoni/pending_submissions_manager.h>

pthread_mutex_t pending_submissions_manager::mtx = PTHREAD_MUTEX_INITIALIZER;

pending_submissions_manager& pending_submissions_manager::get_instance() {
  static pending_submissions_manager instance;
  return instance;
}

void pending_submissions_manager::push(std::string problem_id, int rev_id, std::string submission_id) {
  pthread_mutex_lock(&mtx);
  pending_submissions[problem_id][rev_id].push(submission_id);
  LOG_INFO(std::string("Added pending submission ") + submission_id + " for problem " + problem_id + " rev " + std::to_string(rev_id));
  pthread_mutex_unlock(&mtx);
}

void pending_submissions_manager::problem_failed(std::string problem_id, int rev_id) {
  pthread_mutex_lock(&mtx);
  if (pending_submissions.count(problem_id) && pending_submissions[problem_id].count(rev_id)) {
    while (!pending_submissions[problem_id][rev_id].empty()) {
      std::string submission_id = pending_submissions[problem_id][rev_id].front();
      pending_submissions[problem_id][rev_id].pop();
      LOG_INFO(std::string("Marked submission ") + submission_id + " as failed for problem " + problem_id + " rev " + std::to_string(rev_id));
      submission_manager::get_instance().set_verdict(submission_id, result_enum::FAIL, 0, 0, 0);
    }
  }
  pthread_mutex_unlock(&mtx);
}

void pending_submissions_manager::problem_done(std::string problem_id, int rev_id) {
  pthread_mutex_lock(&mtx);
  if (pending_submissions.count(problem_id) && pending_submissions[problem_id].count(rev_id)) {
    while (!pending_submissions[problem_id][rev_id].empty()) {
      std::string submission_id = pending_submissions[problem_id][rev_id].front();
      pending_submissions[problem_id][rev_id].pop();
      task_queue::get_instance().push(new evaluator_task(submission_id, problem_id, rev_id));
    }
  }
  pthread_mutex_unlock(&mtx);
}

int pending_submissions_manager::count_pending_submissions(std::string problem_id, int rev_id) {
  pthread_mutex_lock(&mtx);
  int count = 0;
  if (pending_submissions.count(problem_id) && pending_submissions[problem_id].count(rev_id)) {
    count = pending_submissions[problem_id][rev_id].size();
  }
  pthread_mutex_unlock(&mtx);
  return count;
}