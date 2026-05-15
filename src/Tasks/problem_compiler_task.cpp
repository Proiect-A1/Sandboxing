#include <Tasks/problem_compiler_task.hpp>

result_enum problem_compiler_task::execute(pthread_t thread_id, int user_id){

  architecture_utilities::clean_run_dir(user_id); // clean run dir before compilation

  stdio_compiler_task* compiler_ptr = stdio_compiler_factory(
    language_enum::CPP, // TODO: determine language from source path
    problem_id + "_rev" + std::to_string(rev_id) + "_" + source_path, // fake submission id
    0 // priority
  );
  stdio_compiler_task compiler = *compiler_ptr;

  general_utilities::copy_file(source_path, architecture_utilities::get_run_dir_absolute_path(user_id) + "/main.cpp", 0755);
  general_utilities::copy_file(architecture_utilities::get_sandbox_path() + "/../headers/problem.h", architecture_utilities::get_run_dir_absolute_path(user_id) + "/problem.h", 0755);

  result_enum rez = compiler.execute(thread_id, user_id);
  if (rez != result_enum::OK){
    LOG_ERROR_USER(user_id, "Problem compiler task failed with result: " + general_utilities::enum_to_string(rez));
    return rez;
  }

  general_utilities::copy_file(architecture_utilities::get_run_dir_absolute_path(user_id) + "/main_exec", source_path.substr(0, source_path.size() - 4), 0755);
  problem_manager::get_instance().add_compiled_source(problem_id, rev_id);
  architecture_utilities::clean_run_dir(user_id); // clean run dir before compilation

  return result_enum::OK;
}