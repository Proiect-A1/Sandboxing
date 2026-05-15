#include <Tasks/problem_compiler_task.hpp>

result_enum problem_compiler_task::execute(pthread_t thread_id, int user_id){
  if (!compiler_task){
    LOG_ERROR_USER(user_id, "Compiler task not set for problem compiler task");
    return result_enum::FAIL;
  }

  result_enum rez = compiler_task->execute(thread_id, user_id);

  if (rez != result_enum::OK){
    LOG_ERROR_USER(user_id, "Problem compiler task failed with result: " + general_utilities::enum_to_string(rez));
    return rez;
  }


  problem_manager::get_instance().add_compiled_source(problem_id, rev_id);

  return result_enum::OK;
}