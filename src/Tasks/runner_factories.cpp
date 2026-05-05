#include <Tasks/runner_factories.hpp>


namespace runner_factories {
  std::unordered_map<language_enum, decltype(&stdio_submission_runner_factory_CPP)> stdio_submission_runner_factory = {
    {language_enum::CPP, stdio_submission_runner_factory_CPP},
  };
  std::unordered_map<language_enum, decltype(&submission_runner_factory_CPP)> submission_runner_factory = {
    {language_enum::CPP, submission_runner_factory_CPP},
  };
  std::unordered_map<language_enum, decltype(&checker_runner_factory_CPP)> checker_runner_factory = {
    {language_enum::CPP, checker_runner_factory_CPP},
  };
  std::unordered_map<language_enum, decltype(&generator_runner_factory_CPP)> generator_runner_factory = {
    {language_enum::CPP, generator_runner_factory_CPP},
  };
  std::unordered_map<language_enum, decltype(&validator_runner_factory_CPP)> validator_runner_factory = {
    {language_enum::CPP, validator_runner_factory_CPP},
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
    uint8_t priority){
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
        run_time_limit,
        run_memory_limit,
        {},
        {},
        {exec_path},
        false);

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
    uint8_t priority){
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
        false);

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
    uint8_t priority){
    if (submission_id.empty() || exec_path.empty() || output_path.empty() || message_path.empty() || submission_input_path.empty() || submission_output_path.empty() || correct_output_path.empty() || source_path.empty() || arguments.empty())
    {
        return nullptr;
    }

    std::vector<std::string> checker_arguments = {exec_path, submission_input_path, submission_output_path, correct_output_path, source_path};
    checker_arguments.insert(checker_arguments.end(), arguments.begin() + 1, arguments.end());
    super_runner_task* created = new super_runner_task(
        submission_id,
        exec_path,
        "/dev/null",
        output_path,
        message_path,
        10.0f, // deocamdata 10s pentru checker mereu!! trebuie discutat daca chiar asa ramane
        1024 * 1024 * 1024, // deocamdata 1GB pentru checker mereu!! trebuie discutat daca chiar asa ramane
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
  uint8_t priority){
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
        10.0f, // deocamdata 10s pentru generator mereu!! trebuie discutat daca chiar asa ramane
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
  const std::string& output_path,
  const std::string& message_path,
  const std::vector<std::string>& arguments,
  uint8_t priority){
    if (submission_id.empty() || exec_path.empty() || input_path.empty() || output_path.empty() || message_path.empty() || arguments.empty())
    {
        return nullptr;
    }
    super_runner_task* created = new super_runner_task(
        submission_id,
        exec_path,
        input_path,
        output_path,
        message_path,
        10.0f, // deocamdata 10s pentru validator mereu!! trebuie discutat daca chiar asa ramane
        1024 * 1024 * 1024, // deocamdata 1GB pentru validator mereu!! trebuie discutat daca chiar asa ramane
        {input_path},
        {output_path},
        arguments,
        true);
    created->priority = priority;
    return created;
}