#include <Tasks/runner_factories.hpp>


namespace runner_factories {
  std::unordered_map<language_enum, decltype(&stdio_submission_runner_factory_CPP)> stdio_submission_runner_factory = {
    {language_enum::CPP, stdio_submission_runner_factory_CPP},
    {language_enum::COMPILED, stdio_submission_runner_factory_CPP},
    {language_enum::RUST, stdio_submission_runner_factory_CPP},
    {language_enum::C, stdio_submission_runner_factory_CPP},
    {language_enum::CSHARP, stdio_submission_runner_factory_CPP},
    {language_enum::GO, stdio_submission_runner_factory_CPP},
    {language_enum::PYTHON, stdio_submission_runner_factory_CPP},
  };
  std::unordered_map<language_enum, decltype(&submission_runner_factory_CPP)> submission_runner_factory = {
    {language_enum::CPP, submission_runner_factory_CPP},
    {language_enum::COMPILED, submission_runner_factory_CPP},
    {language_enum::RUST, submission_runner_factory_CPP},
    {language_enum::C, submission_runner_factory_CPP},
    {language_enum::CSHARP, submission_runner_factory_CPP},
    {language_enum::GO, submission_runner_factory_CPP},
    {language_enum::PYTHON, submission_runner_factory_CPP},
  };
  std::unordered_map<language_enum, decltype(&checker_runner_factory_CPP)> checker_runner_factory = {
    {language_enum::CPP, checker_runner_factory_CPP},
    {language_enum::COMPILED, checker_runner_factory_CPP},
    {language_enum::RUST, checker_runner_factory_CPP},
    {language_enum::C, checker_runner_factory_CPP},
    {language_enum::CSHARP, checker_runner_factory_CPP},
    {language_enum::GO, checker_runner_factory_CPP},
    {language_enum::PYTHON, checker_runner_factory_CPP},
  };
  std::unordered_map<language_enum, decltype(&generator_runner_factory_CPP)> generator_runner_factory = {
    {language_enum::CPP, generator_runner_factory_CPP},
    {language_enum::COMPILED, generator_runner_factory_CPP},
    {language_enum::RUST, generator_runner_factory_CPP},
    {language_enum::C, generator_runner_factory_CPP},
    {language_enum::CSHARP, generator_runner_factory_CPP},
    {language_enum::GO, generator_runner_factory_CPP},
    {language_enum::PYTHON, generator_runner_factory_CPP},
  };
  std::unordered_map<language_enum, decltype(&validator_runner_factory_CPP)> validator_runner_factory = {
    {language_enum::CPP, validator_runner_factory_CPP},
    {language_enum::COMPILED, validator_runner_factory_CPP},
    {language_enum::RUST, validator_runner_factory_CPP},
    {language_enum::C, validator_runner_factory_CPP},
    {language_enum::CSHARP, validator_runner_factory_CPP},
    {language_enum::GO, validator_runner_factory_CPP},
    {language_enum::PYTHON, validator_runner_factory_CPP},
  };
}

// cine implementeaza language support sa se simta liber sa le organizeze cum vrea, atata timp cat respecta comandamentele lui iggy
super_runner_task* runner_factories::stdio_submission_runner_factory_CPP(
    const std::string& submission_id,
    const std::string& exec_path,
    const std::string& input_path,
    const std::string& output_path,
    float run_time_limit,
    long long run_memory_limit,
    uint8_t priority = 0,
    bool strong_user = false){
    if (submission_id.empty() || exec_path.empty() || input_path.empty() || output_path.empty())
    {
        return nullptr;
    }

    super_runner_task* created = new super_runner_task(
        submission_id,
        exec_path,
        input_path,
        output_path,
        "/dev/null",
        //output_path, // pt debug in loc de /dev/null
        run_time_limit,
        run_memory_limit,
        {},
        {},
        {exec_path},
        strong_user);

    created->priority = priority;
    return created;
}

super_runner_task* runner_factories::submission_runner_factory_CPP(
    const std::string& submission_id,
    const std::string& exec_path,
    const std::string& input_path,
    const std::string& output_path,
    float run_time_limit,
    long long run_memory_limit,
    uint8_t priority = 0,
    bool strong_user = false){
    if (submission_id.empty() || exec_path.empty() || input_path.empty() || output_path.empty())
    {
        return nullptr;
    }

    super_runner_task* created = new super_runner_task(
        submission_id,
        exec_path,
        "/dev/null",
        "/dev/null",
        "/dev/null",
        run_time_limit,
        run_memory_limit,
        {input_path},
        {output_path},
        {exec_path},
        strong_user);

    created->priority = priority;
    return created;
}

super_runner_task* runner_factories::checker_runner_factory_CPP(
    const std::string& submission_id,
    const std::string& exec_path,
    const std::string& output_path,
    const std::string& message_path,
    const std::string& submission_input_path,
    const std::string& submission_output_path,
    const std::string& correct_output_path,
    const std::string& source_path,
    const std::vector<std::string>& arguments,
    uint8_t priority = 0){
    if (submission_id.empty() || exec_path.empty() || output_path.empty() || message_path.empty() || submission_input_path.empty() || submission_output_path.empty() || correct_output_path.empty() || source_path.empty() || arguments.empty())
    {
        return nullptr;
    }

    std::vector<std::string> checker_arguments = {exec_path, submission_input_path, submission_output_path, correct_output_path, source_path};
    checker_arguments.insert(checker_arguments.end(), arguments.begin() + 1, arguments.end());
    std::string checker_args_str;
    for (const std::string& arg : checker_arguments) {
      checker_args_str += arg + " ";
    }
    LOG_DEBUG("Checker arguments: " + checker_args_str);
    super_runner_task* created = new super_runner_task(
        submission_id,
        exec_path,
        "/dev/null",
        output_path,
        message_path,
        10000.0f, // deocamdata 10s pentru checker mereu!! trebuie discutat daca chiar asa ramane
        1024ll * 1024 * 1024, // deocamdata 1GB pentru checker mereu!! trebuie discutat daca chiar asa ramane
        {submission_input_path, submission_output_path, correct_output_path, source_path},
        {},
        checker_arguments,
        true);

    created->priority = priority;
    return created;
}

super_runner_task* runner_factories::generator_runner_factory_CPP(
  const std::string& submission_id,
  const std::string& exec_path,
  const std::string& output_path,
  const std::string& message_path,
  const std::vector<std::string>& arguments,
  uint8_t priority = 0){
    if (submission_id.empty() || exec_path.empty() || output_path.empty() || message_path.empty() || arguments.empty())
    {
        return nullptr;
    }
    super_runner_task* created = new super_runner_task(
        submission_id,
        exec_path,
        "/dev/null",
        output_path,
        message_path,
        10000.0f, // deocamdata 10s pentru generator mereu!! trebuie discutat daca chiar asa ramane
        1024 * 1024 * 1024, // deocamdata 1GB pentru generator mereu!! trebuie discutat daca chiar asa ramane
        {},
        {},
        arguments,
        true);
    created->priority = priority;
    return created;

}

super_runner_task* runner_factories::validator_runner_factory_CPP(
  const std::string& submission_id,
  const std::string& exec_path,
  const std::string& input_path,
  const std::string& message_path,
  const std::vector<std::string>& arguments,
  uint8_t priority = 0){
  if (submission_id.empty() || exec_path.empty() || input_path.empty() || message_path.empty() || arguments.empty())
  {
      return nullptr;
  }
  super_runner_task* created = new super_runner_task(
      submission_id,
      exec_path,
      input_path,
      "/dev/null",
      message_path,
      10000.0f, // deocamdata 10s pentru validator mereu!! trebuie discutat daca chiar asa ramane
      1024 * 1024 * 1024, // deocamdata 1GB pentru validator mereu!! trebuie discutat daca chiar asa ramane
      {},
      {},
      arguments,
      true);
    created->priority = priority;
    return created;
}