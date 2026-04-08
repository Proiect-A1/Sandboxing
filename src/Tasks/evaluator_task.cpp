#include <Tasks/evaluator_task.h> 
result_enum evaluator_task::execute(int thread_id, int user_id) {
  if (user_id <= 0) {
    print_error(thread_id, user_id, "Invalid user ID");
    return result_enum::FAIL;
  }
  result_enum result;
  problem_manager& pm = problem_manager::get_instance();




  auto compiler = stdio_compiler_factory(
    language_enum::CPP,
    submission_id,
    0 // priority
  );

  if (!compiler) {
    print_error(thread_id, user_id, "Failed to create compiler task");
    return result_enum::FAIL;
  }

  result = compiler->execute(thread_id, user_id);
  if (result != result_enum::OK) {
    return result;
  }

  problem_metadata problem = pm.get_metadata(problem_id, rev_id);
  std::vector<result_enum> test_results(problem.test_count);
  for (int test_id = 0; test_id < problem.test_count; test_id++) {
    test_results[test_id] = stdio_grader_task(
      submission_id,
      problem_id,
      rev_id,
      test_id
    ).execute(thread_id, user_id);
    std::cout << "Test " << test_id << ": ";
    switch(test_results[test_id]) {
      case result_enum::OK: std::cout << "OK"; break;
      case result_enum::WA: std::cout << "WA"; break;
      case result_enum::PA: std::cout << "PA"; break;
      case result_enum::TLE: std::cout << "TLE"; break;
      case result_enum::MLE: std::cout << "MLE"; break;
      case result_enum::RTE: std::cout << "RTE"; break;
      case result_enum::CPE: std::cout << "CPE"; break;
      case result_enum::FAIL: std::cout << "FAIL"; break;
      case result_enum::SKIP: std::cout << "SKIP"; break;
      case result_enum::ILE: std::cout << "ILE"; break;
      case result_enum::NONE: std::cout << "NONE"; break;
      case result_enum::OTHER: std::cout << "OTHER"; break;
    }
    std::cout << std::endl;
    if (test_results[test_id] == result_enum::FAIL) {
      return result_enum::FAIL;
    }
  }

  std::map<result_enum, int> result_counts;
  for (int i = 0; i < problem.test_count; i++) {
    result_counts[test_results[i]]++;
  }

  for (auto [key, val] : result_counts){
    std::cout << "Result ";
    switch (key) {
      case result_enum::OK: std::cout << "OK"; break;
      case result_enum::WA: std::cout << "WA"; break;
      case result_enum::PA: std::cout << "PA"; break;
      case result_enum::TLE: std::cout << "TLE"; break;
      case result_enum::MLE: std::cout << "MLE"; break;
      case result_enum::RTE: std::cout << "RTE"; break;
      case result_enum::CPE: std::cout << "CPE"; break;
      case result_enum::FAIL: std::cout << "FAIL"; break;
      case result_enum::SKIP: std::cout << "SKIP"; break;
      case result_enum::ILE: std::cout << "ILE"; break;
      case result_enum::NONE: std::cout << "NONE"; break;
      case result_enum::OTHER: std::cout << "OTHER"; break;
    }
    std::cout  << ": " << val << std::endl;
  }

  return result_enum::OK;

}

void evaluator_task:: print_error(int thread_id, int user_id,const std::string& message){
  fprintf(stderr, "Evaluator task running on thread %d, with user %d: %s\n", thread_id, user_id, message.c_str());
}