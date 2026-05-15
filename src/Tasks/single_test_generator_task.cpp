#include <Tasks/single_test_generator_task.hpp>

single_test_generator_task::stgt_helper::~stgt_helper() {
  if (architecture_utilities::clean_run_dir(user_id) != 0){
    LOG_ERROR_USER(user_id, "Failed to clean up run directory from helper");
  }
  else{
    LOG_DEBUG_USER(user_id, "Successfully cleaned up run directory from helper");
  }

  problem_manager& pm = problem_manager::get_instance(); 
  if (result != result_enum::OK){
    LOG_ERROR_USER(user_id, "Test generation finished with NON-OK result: " + general_utilities::enum_to_string(result));
    pm.update_problem_status(problem_id, rev_id, problem_status_enum::FAILED);
  }
  else{
    LOG_INFO_USER(user_id, "Test generation finished successfully");
    pm.add_generated_test(problem_id, rev_id);
  }
}

result_enum single_test_generator_task::execute(pthread_t thread_id, int user_id){

  //ROBERT SCOATE POINTERII DE LA RUNNER_FACTORIES CA MEMORY LEAKS ROBERT
  
  if (user_id <= 0){
    LOG_ERROR_USER(user_id, "Invalid user ID");
    // de bagat in problem manager ca o dat fail
    return result_enum::FAIL;
  }
  
  stgt_helper helper(user_id, problem_id, rev_id, test_id);
  helper.result = result_enum::FAIL;

  if (architecture_utilities::clean_run_dir(user_id) != 0){
    LOG_ERROR_USER(user_id, "Failed to clean up run directory");
    return result_enum::FAIL;
  }
  if (problem_id.empty()){
    LOG_ERROR_USER(user_id, "Problem id not set");
    return result_enum::FAIL;
  }
  
  if (test_meta.generator_args.empty()){
    LOG_ERROR_USER(user_id, "Generator arguments not set");
    return result_enum::FAIL;
  }
  if (test_meta.validator_args.empty()){
    LOG_ERROR_USER(user_id, "Validator arguments not set");
    return result_enum::FAIL;
  }
  if (test_meta.checker_args.empty()){
    LOG_ERROR_USER(user_id, "Checker arguments not set");
    return result_enum::FAIL;
  }

  // generam testu, validam rezultatul, checker-uieste rezultatul, mutam testu generat in locul potrivit
  std::string run_dir = architecture_utilities::get_run_dir_absolute_path(user_id);

  std::string source_name = test_meta.source_path;
  std::string main_exec_name = test_meta.main_path;
  std::string generator_exec_name = test_meta.generator_args[0];

  std::string validator_exec_name = (test_meta.validator_args.empty() ? std::string(0, '0') : test_meta.validator_args[0]);
  std::string checker_exec_name = test_meta.checker_args[0];

  std::string source_path = architecture_utilities::get_problem_source_exec_path(problem_id, rev_id, source_name);

  std::string main_exec_path = architecture_utilities::get_problem_source_exec_path(problem_id, rev_id, main_exec_name);
  std::string correct_output_path = run_dir + "/correct_output";

  std::string generator_exec_path = architecture_utilities::get_problem_generator_exec_path(problem_id, rev_id, generator_exec_name);
  std::string generator_output_1_path = run_dir + "/generator_output_1";
  std::string generator_output_2_path = run_dir + "/generator_output_2";
  std::string generator_message_1_path = run_dir + "/generator_message_1";
  std::string generator_message_2_path = run_dir + "/generator_message_2";

  std::string validator_exec_path = architecture_utilities::get_problem_validator_exec_path(problem_id, rev_id, validator_exec_name);
  std::string validator_message_path = run_dir + "/validator_message";

  std::string checker_exec_path   = architecture_utilities::get_problem_validator_exec_path(problem_id, rev_id, checker_exec_name);
  std::string checker_output_path = run_dir + "/checker_output";
  std::string checker_message_path = run_dir + "/checker_message";


  std::string fake_submission_id = problem_id + "_rev" + std::to_string(rev_id) + "_test" + std::to_string(test_id); 



  if (generator_exec_name == "="){
    general_utilities::copy_file(architecture_utilities::get_problem_raw_test_path(problem_id, rev_id, test_meta.generator_args[1]), generator_output_1_path, 0755);
  }
  else{
    general_utilities::copy_file(generator_exec_path, run_dir + "/" + generator_exec_name, 0755);
    auto generator_task_1_ptr = runner_factories::generator_runner_factory[language_enum::COMPILED](
      fake_submission_id,
      test_meta.generator_args[0],
      generator_output_1_path,
      generator_message_1_path,
      test_meta.generator_args,
      1 // priority
    );
    if (!generator_task_1_ptr) {
      LOG_ERROR_USER(user_id, "Failed to create generator task 1");
      return result_enum::FAIL;
    }
    auto generator_task_1 = *generator_task_1_ptr;
    delete generator_task_1_ptr;
    auto generator_task_2_ptr = runner_factories::generator_runner_factory[language_enum::COMPILED](
      fake_submission_id,
      test_meta.generator_args[0],
      generator_output_2_path,
      generator_message_2_path,
      test_meta.generator_args,
      1 // priority
    );
    if (!generator_task_2_ptr) {
      LOG_ERROR_USER(user_id, "Failed to create generator task 2");
      return result_enum::FAIL;
    }
    auto generator_task_2 = *generator_task_2_ptr;
    delete generator_task_2_ptr;
    result_enum aux_rez = generator_task_1.execute(thread_id, user_id);

    if (aux_rez != result_enum::OK){
      helper.result = aux_rez;
      LOG_INFO_USER(user_id, "Generator_1 finished with NON-OK result: " + general_utilities::enum_to_string(aux_rez));
      return aux_rez;
    }
    aux_rez = generator_task_2.execute(thread_id, user_id);

    if (aux_rez != result_enum::OK){
      helper.result = aux_rez;
      LOG_INFO_USER(user_id, "Generator_2 finished with NON-OK result: " + general_utilities::enum_to_string(aux_rez));
      return aux_rez;
    }

    std::string check_command = "diff -q " + generator_output_1_path + " " + generator_output_2_path; 
    if (system(check_command.c_str()) != 0){
      helper.result = result_enum::FAIL;
      LOG_INFO_USER(user_id, "Generator is non deterministic");
      return result_enum::FAIL;
    }

    //de inlocuit cu un hashsum, care trebuie retinut undeva ca sa verificam teste duplicate
    LOG_ERROR_USER(user_id, "ROBERT BAGA HASHSUM NU FI LENES (not actual error dar sa sara in ochi)");
  }

  
  if (!validator_exec_name.empty()){
    general_utilities::copy_file(validator_exec_path, run_dir + "/" + validator_exec_name, 0755);
    auto validator_task_ptr = runner_factories::validator_runner_factory[language_enum::COMPILED](
      fake_submission_id,
      validator_exec_name,
      generator_output_1_path,
      validator_message_path,
      test_meta.validator_args,
      1
    );
  
    if (!validator_task_ptr) {
      LOG_ERROR_USER(user_id, "Failed to create validator task");
      return result_enum::FAIL;
    }
    auto validator_task = *validator_task_ptr;
    delete validator_task_ptr;
    result_enum aux_rez = validator_task.execute(thread_id, user_id);

    if (aux_rez != result_enum::OK && aux_rez != result_enum::RTE){
      helper.result = aux_rez;
      LOG_ERROR_USER(user_id, "Validator finished with NON-OK result: " + general_utilities::enum_to_string(aux_rez));
      return aux_rez;
    }
    if (aux_rez == result_enum::RTE){
      switch(validator_task.get_exit_code()){
        case 1:
        break;


        default:
        return result_enum::RTE;
      }
    }
  }

  general_utilities::copy_file(main_exec_path, run_dir + "/" + main_exec_name, 0755);

  auto source_task_ptr = runner_factories::stdio_submission_runner_factory[language_enum::COMPILED](
    fake_submission_id,
    main_exec_name,
    generator_output_1_path,
    correct_output_path,
    time_limit,
    memory_limit,
    0,
    true
  );
  if (!source_task_ptr) {
    LOG_ERROR_USER(user_id, "Failed to create source task");
    return result_enum::FAIL;
  }
  auto source_task = *source_task_ptr;
  delete source_task_ptr;

  result_enum aux_rez = source_task.execute(thread_id, user_id);
  if (aux_rez != result_enum::OK){
    helper.result = aux_rez;
    LOG_ERROR_USER(user_id, "Main source finished with NON-OK result: " + general_utilities::enum_to_string(aux_rez));
    return aux_rez;
  }

  auto checker = checker_task(
    fake_submission_id,
    generator_output_1_path,
    correct_output_path,
    correct_output_path,
    source_name
  );

  aux_rez = checker.execute(thread_id, user_id);

  if (aux_rez != result_enum::OK){
    helper.result = aux_rez;
    LOG_ERROR_USER(user_id, "Checker finished with NON-OK result: " + general_utilities::enum_to_string(aux_rez));
    return aux_rez;
  }

  if (!general_utilities::copy_file(generator_output_1_path, architecture_utilities::get_problem_input_path(problem_id, rev_id, test_id), 0755)){
    LOG_ERROR_USER(user_id, "Failed to copy generator output to problem input path");
    return result_enum::FAIL;
  }
  if (!general_utilities::copy_file(correct_output_path, architecture_utilities::get_problem_correct_output_path(problem_id, rev_id, test_id), 0755)){
    LOG_ERROR_USER(user_id, "Failed to copy correct output to problem correct output path");
    return result_enum::FAIL;
  }
  if (!general_utilities::copy_file(generator_output_1_path, architecture_utilities::get_problem_tests_inputs_folder(problem_id, rev_id) + "/" + general_utilities::left_zero_pad(test_id, 3) + ".in", 0755)){
    LOG_ERROR_USER(user_id, "Failed to copy generator output to tests folder");
    return result_enum::FAIL;
  }
  if (!general_utilities::copy_file(correct_output_path, architecture_utilities::get_problem_tests_correct_outputs_folder(problem_id, rev_id) + "/" + general_utilities::left_zero_pad(test_id, 3) + ".ok", 0755)){
    LOG_ERROR_USER(user_id, "Failed to copy correct output to tests folder");
    return result_enum::FAIL;
  }

  helper.result = result_enum::OK;
  return result_enum::OK;
}