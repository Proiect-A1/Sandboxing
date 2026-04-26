#include <Singletoni/problem_manager.h>
#include <Singletoni/logger.h>

pthread_mutex_t problem_manager::mtx = PTHREAD_MUTEX_INITIALIZER;

problem_manager& problem_manager::get_instance() {
  static problem_manager instance;
  return instance;
}

void problem_manager::add_revision(problem_metadata metadata) {
  pthread_mutex_lock(&mtx);
  problems[metadata.problem_id][metadata.rev_id] = metadata;
  LOG_INFO(std::string("Added revision ") + std::to_string(metadata.rev_id) + " for problem " + metadata.problem_id);
  pthread_mutex_unlock(&mtx);
}

void problem_manager::delete_revision(std::string problem_id, int rev_id) {
  pthread_mutex_lock(&mtx);
  if (problems.count(problem_id) && problems[problem_id].count(rev_id)) {
    // trebe sters si folderu
    problems[problem_id].erase(rev_id);
    if (problems[problem_id].empty()) {
      // trebe sters si folderu
      problems.erase(problem_id);
    }
    LOG_INFO(std::string("Deleted revision ") + std::to_string(rev_id) + " for problem " + problem_id);
  } 
  else {
    LOG_WARNING(std::string("Attempted to delete missing revision ") + std::to_string(rev_id) + " for problem " + problem_id);
  }
  pthread_mutex_unlock(&mtx);
}

int problem_manager::delete_old_revisions(std::string problem_id, int keep_count) {
  pthread_mutex_lock(&mtx);
  if (problems.count(problem_id)) {
    auto& revisions = problems[problem_id];
    while (revisions.size() > keep_count) {
      // trebe sters si folderu
      auto it = revisions.begin();
      revisions.erase(it);
    }
    if (revisions.empty()) {
      // trebe sters si folderu
      problems.erase(problem_id);
    }
    LOG_INFO(std::string("Deleted old revisions for problem ") + problem_id);
    pthread_mutex_unlock(&mtx);
    return revisions.size();
  }
  LOG_WARNING(std::string("Attempted to delete revisions for missing problem ") + problem_id);
  pthread_mutex_unlock(&mtx);
  return 0;
}

int problem_manager::get_latest_revision(std::string problem_id) {
  pthread_mutex_lock(&mtx);
  if (problems.count(problem_id) && !problems[problem_id].empty()) {
    auto& revisions = problems[problem_id];
    pthread_mutex_unlock(&mtx);
    return revisions.rbegin()->first; // return the latest revision id
  }
  LOG_WARNING(std::string("No latest revision found for problem ") + problem_id);
  pthread_mutex_unlock(&mtx);
  return -1; // return -1 if problem does not exist or has no revisions
}

bool problem_manager::exists_revision(std::string problem_id, int rev_id) {
  pthread_mutex_lock(&mtx);
  bool exists = problems.count(problem_id) && problems[problem_id].count(rev_id);
  pthread_mutex_unlock(&mtx);
  return exists;
}

int problem_manager::count_revisions(std::string problem_id) {
  pthread_mutex_lock(&mtx);
  int count = problems.count(problem_id) ? problems[problem_id].size() : 0;
  pthread_mutex_unlock(&mtx);
  return count;
}

problem_metadata problem_manager::get_metadata(std::string problem_id, int rev_id) {
  pthread_mutex_lock(&mtx);
  if (problems.count(problem_id) && problems[problem_id].count(rev_id)) {
    auto metadata = problems[problem_id][rev_id];
    pthread_mutex_unlock(&mtx);
    return metadata;
  }
  LOG_WARNING(std::string("Metadata not found for problem ") + problem_id + " rev " + std::to_string(rev_id));
  pthread_mutex_unlock(&mtx);
  return problem_metadata(); // return default metadata if problem or revision does not exist
}