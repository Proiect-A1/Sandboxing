#ifndef PROBLEM_MANAGER_H
#define PROBLEM_MANAGER_H

#include <Chestii_cu_data/problem_metadata.h>
#include <map>
#include <pthread.h>
#include <sys/mman.h>
#include <string.h>
#include <unordered_map>

class problem_manager {
  //Singleton
  static problem_manager* instance;
  static pthread_mutex_t mtx;
  std::map<std::string, std::map<int, problem_metadata>> problems; // problem_id -> revizion_id -> metadata
  problem_manager() = default;
  public:
  static problem_manager& get_instance();
  void add_revision(std::string problem_id, int rev_id, problem_metadata metadata);
  void delete_revision(std::string problem_id, int rev_id);
  int delete_old_revisions(std::string problem_id, int keep_count = 1); // keep only the latest keep_count revisions, default is 1, returns number of revisions left
  int get_latest_revision(std::string problem_id);
  bool exists_revision(std::string problem_id, int rev_id);
  int count_revisions(std::string problem_id);
  problem_metadata get_metadata(std::string problem_id, int rev_id);

};



#endif // PROBLEM_MANAGER_H