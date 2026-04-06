// TODO: DE TINUT MINTE CATE SURSE RULEAZA PE FIECARE REVIZIE
#ifndef PROBLEM_MANAGER_H
#define PROBLEM_MANAGER_H

#include <Chestii_cu_data/problem_metadata.h>

#include <pthread.h>
#include <unordered_map>
#include <map>
class problem_manager {
  //Singleton
  static problem_manager* instance;
  static pthread_mutex_t mtx;
  std::unordered_map<int, std::map<int, problem_metadata>> problems; // problem_id -> revizion_id -> metadata
  problem_manager() = default;
  public:
  static problem_manager& get_instance();
  void add_revision(int problem_id, int rev_id, problem_metadata metadata);
  void delete_revision(int problem_id, int rev_id);
  int delete_old_revisions(int problem_id, int keep_count = 1); // keep only the latest keep_count revisions, default is 1, returns number of revisions left
  int get_latest_revision(int problem_id);
  bool exists_revision(int problem_id, int rev_id);
  int count_revisions(int problem_id);
  problem_metadata get_metadata(int problem_id, int rev_id);

};



#endif // PROBLEM_MANAGER_H