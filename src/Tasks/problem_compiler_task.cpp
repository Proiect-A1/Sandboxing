#include <Tasks/problem_compiler_task.h>

problem_compiler_task::pct_helper::~pct_helper() {
  if (architecture_utilities::clean_run_dir(user_id) != 0){
    LOG_ERROR_USER(user_id, "Failed to clean up run directory from helper");
  }
  else{
    LOG_DEBUG_USER(user_id, "Successfully cleaned up run directory from helper");
  }

  LOG_DEBUG_USER(user_id, "in the pct time: " + problem_manager::get_instance().get_metadata(problem_id , rev_id).founding_submission_id + "   problem_id  " + problem_manager::get_instance().get_metadata(problem_id , rev_id).problem_id + "   " + std::to_string(problem_manager::get_instance().get_metadata(problem_id , rev_id).rev_id));
    
  if (result != result_enum::OK){
    LOG_ERROR_USER(user_id, "Problem compilation finished with NON-OK result: " + general_utilities::enum_to_string(result));
    problem_manager::get_instance().update_problem_status(problem_id, rev_id, problem_status_enum::FAILED);
  }
  else{
    LOG_INFO_USER(user_id, "Problem compilation finished successfully");
  }
}

result_enum problem_compiler_task::execute(pthread_t thread_id, int user_id){
  LOG_DEBUG_USER(user_id, "Starting problem compilation for problem " + problem_id + " revision " + std::to_string(rev_id));

  pct_helper helper(user_id, problem_id, rev_id);

  if (architecture_utilities::clean_run_dir(user_id) != 0) { // clean run dir before compilation
    LOG_ERROR_USER(user_id, "Failed to clean run directory before compilation");
    return result_enum::FAIL;
  }

  LOG_DEBUG_USER(user_id, "Initializing compiler for problem compilation");
  stdio_compiler_task* compiler_ptr = stdio_compiler_factory(
    language_enum::CPP, // TODO: determine language from source path
    problem_id + "_rev" + std::to_string(rev_id) + "_" + source_path, // fake submission id
    0 // priority
  );
  stdio_compiler_task compiler = *compiler_ptr;
  LOG_DEBUG_USER(user_id, "Compiler initialized successfully for problem compilation");
  delete compiler_ptr;

  if (!general_utilities::copy_file(source_path, architecture_utilities::get_run_dir_absolute_path(user_id) + "/main.cpp", 0755)) {
    LOG_ERROR_USER(user_id, "Failed to copy source file");
    return result_enum::FAIL;
  }
  if (!general_utilities::copy_file(architecture_utilities::get_sandbox_path() + "/../headers/problem.h", architecture_utilities::get_run_dir_absolute_path(user_id) + "/problem.h", 0755)) {
    LOG_ERROR_USER(user_id, "Failed to copy problem.h file");
    return result_enum::FAIL;
  }

  LOG_DEBUG_USER(user_id, "Starting compilation process for problem compilation");
  result_enum rez = compiler.execute(thread_id, user_id);
  if (rez != result_enum::OK){
    LOG_ERROR_USER(user_id, "Problem compiler task failed with result: " + general_utilities::enum_to_string(rez));
    problem_manager::get_instance().update_problem_status(problem_id, rev_id, problem_status_enum::FAILED);
    return rez;
  }
  LOG_DEBUG_USER(user_id, "Compilation process finished successfully for problem compilation");

  if (!general_utilities::copy_file(architecture_utilities::get_run_dir_absolute_path(user_id) + "/main_exec", source_path.substr(0, source_path.size() - 4), 0755)) {
    LOG_ERROR_USER(user_id, "Failed to copy compiled executable");
    return result_enum::FAIL;
  }
  problem_manager::get_instance().add_compiled_source(problem_id, rev_id);
  LOG_DEBUG_USER(user_id, "Finished problem compilation for problem " + problem_id + " revision " + std::to_string(rev_id));
  helper.result = result_enum::OK;
  return result_enum::OK;
}