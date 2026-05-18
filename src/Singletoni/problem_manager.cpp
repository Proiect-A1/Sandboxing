#include <Singletoni/problem_manager.h>
#include <Singletoni/logger.h>
#include <Tasks/upload.h>
#include <Server/IO.hpp>

pthread_mutex_t problem_manager::mtx = PTHREAD_MUTEX_INITIALIZER;

problem_manager& problem_manager::get_instance() {
  static problem_manager instance;
  return instance;
}

void problem_manager::add_revision(problem_metadata metadata) {
  pthread_mutex_lock(&mtx);
  problems[metadata.problem_id][metadata.rev_id] = metadata;
  std::cerr << "founding submission id is: " << metadata.founding_submission_id << std::endl;
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

problem_metadata problem_manager::get_metadata_unsafe(std::string problem_id, int rev_id) {
  if (problems.count(problem_id) && problems[problem_id].count(rev_id)) {
    auto metadata = problems[problem_id][rev_id];
    return metadata;
  }
  LOG_WARNING(std::string("Metadata not found for problem ") + problem_id + " rev " + std::to_string(rev_id));
  return problem_metadata(); // return default metadata if problem or revision does not exist
}

void problem_manager::update_problem_status(std::string problem_id , int rev_id , problem_status_enum problem_status)
{
    pthread_mutex_lock(&mtx);
    
    if (problems.count(problem_id) && problems[problem_id].count(rev_id)) {
      problems[problem_id][rev_id].problem_status = problem_status;
      pthread_mutex_unlock(&mtx);
      return;
    }

    LOG_WARNING(std::string("Metadata not found for problem ") + problem_id + " rev " + std::to_string(rev_id));
    pthread_mutex_unlock(&mtx);
}

problem_status_enum problem_manager::get_problem_status(std::string problem_id , int rev_id)
{
    pthread_mutex_lock(&mtx);
    
    if (problems.count(problem_id) && problems[problem_id].count(rev_id)) {
      problem_status_enum result = problems[problem_id][rev_id].problem_status; 
      pthread_mutex_unlock(&mtx);
      return result;
    }

    LOG_WARNING(std::string("Metadata not found for problem ") + problem_id + " rev " + std::to_string(rev_id));
    pthread_mutex_unlock(&mtx);
    return problem_status_enum::NOT_EXISTS;
}

void problem_manager::start_generating_tests(std::string problem_id, int rev_id){
  pthread_mutex_lock(&mtx);
    
  if (problems.count(problem_id) && problems[problem_id].count(rev_id)) {
    problems[problem_id][rev_id].problem_status = problem_status_enum::GENERATING;
    problems[problem_id][rev_id].tests_to_generate_count = problems[problem_id][rev_id].test_count;
    LOG_INFO(std::string("Started generating tests for problem ") + problem_id + " rev " + std::to_string(rev_id) + " with " + std::to_string(problems[problem_id][rev_id].test_count) + " tests to generate");
    pthread_mutex_unlock(&mtx);
    return;
  }

  LOG_WARNING(std::string("Metadata not found for problem ") + problem_id + " rev " + std::to_string(rev_id));
  pthread_mutex_unlock(&mtx);
}
void problem_manager::add_generated_test(std::string problem_id, int rev_id){
  pthread_mutex_lock(&mtx);
  if (problems.count(problem_id) && problems[problem_id].count(rev_id)) {
    LOG_INFO("YOOOO gata inca un test " + std::to_string(problems[problem_id][rev_id].tests_to_generate_count) + " ramase pentru problem " + problem_id + " rev " + std::to_string(rev_id));
    problems[problem_id][rev_id].tests_to_generate_count--;
    LOG_INFO("YOOOO gata inca un test " + std::to_string(problems[problem_id][rev_id].tests_to_generate_count) + " ramase pentru problem " + problem_id + " rev " + std::to_string(rev_id));
    std::cerr << "founding submission id: " << problems[problem_id][rev_id].founding_submission_id << "   problem_id  " << problems[problem_id][rev_id].problem_id << "   " << problems[problem_id][rev_id].rev_id << std::endl;
    
    if (problems[problem_id][rev_id].tests_to_generate_count <= 0){
      problems[problem_id][rev_id].problem_status = problem_status_enum::DONE;
      LOG_INFO(std::string("All tests generated for problem ") + problem_id + " rev " + std::to_string(rev_id) + ". Problem is now DONE.");

      problem_metadata requested_problem_metadata = problems[problem_id][rev_id];
      submission_data founding_submission_data = submission_manager::get_instance().get_submission(requested_problem_metadata.founding_submission_id);
      upload_task *upl = new upload_task(founding_submission_data.upload_link , problem_id , rev_id);
      task_queue::get_instance().push(upl);

      IO::upload_tests_request(problem_id , rev_id , requested_problem_metadata.tests , requested_problem_metadata.groups , founding_submission_data.socket_fd);
    }
  }
  else {
    LOG_WARNING(std::string("Metadata not found for problem ") + problem_id + " rev " + std::to_string(rev_id));
  }

  pthread_mutex_unlock(&mtx);
}

void problem_manager::start_compiling_sources(std::string problem_id, int rev_id, int sources_to_compile_count){
  pthread_mutex_lock(&mtx);
    
  if (problems.count(problem_id) && problems[problem_id].count(rev_id)) {
    std::cerr << "founding submission id: started compiling" << problems[problem_id][rev_id].founding_submission_id << "   problem_id  " << problems[problem_id][rev_id].problem_id << "   " << problems[problem_id][rev_id].rev_id << std::endl;

    problems[problem_id][rev_id].problem_status = problem_status_enum::COMPILING;
    problems[problem_id][rev_id].sources_to_compile_count = sources_to_compile_count;
    LOG_INFO(std::string("Started compiling sources for problem ") + problem_id + " rev " + std::to_string(rev_id));
    pthread_mutex_unlock(&mtx);
    return;
  }

  LOG_WARNING(std::string("Metadata not found for problem ") + problem_id + " rev " + std::to_string(rev_id));
  pthread_mutex_unlock(&mtx);
}

void problem_manager::add_compiled_source(std::string problem_id, int rev_id){
  pthread_mutex_lock(&mtx);
    
  if (problems.count(problem_id) && problems[problem_id].count(rev_id)) {
       std::cerr << "founding submission id: add compiled factor" << problems[problem_id][rev_id].founding_submission_id << "   problem_id  " << problems[problem_id][rev_id].problem_id << "   " << problems[problem_id][rev_id].rev_id << std::endl;
    problems[problem_id][rev_id].sources_to_compile_count--;
    LOG_DEBUG("Compiled source for problem " + problem_id + " rev " + std::to_string(rev_id) + ". " + std::to_string(problems[problem_id][rev_id].sources_to_compile_count) + " sources left to compile.");
    if (problems[problem_id][rev_id].sources_to_compile_count <= 0){
      problems[problem_id][rev_id].problem_status = problem_status_enum::GENERATING;
      LOG_INFO(std::string("All sources compiled for problem ") + problem_id + " rev " + std::to_string(rev_id) + ". Problem is now GENERATING.");
      task_queue::get_instance().push(new generator_task(problem_id, rev_id));
      LOG_INFO(std::string("Pushed generator task for problem ") + problem_id + " rev " + std::to_string(rev_id) + " to task queue.");
    }
  }
  else {
    LOG_WARNING(std::string("Metadata not found for problem ") + problem_id + " rev " + std::to_string(rev_id));
  }

  pthread_mutex_unlock(&mtx);
}