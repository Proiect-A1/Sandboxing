#include <Tasks/runner_factories.hpp>


namespace runner_factories {
  std::unordered_map<language_enum, decltype(stdio_submission_runner_factory_CPP)> stdio_submission_runner_factory = {
    {language_enum::CPP, stdio_submission_runner_factory_CPP},
  };
  std::unordered_map<language_enum, decltype(submission_runner_factory_CPP)> submission_runner_factory = {
    {language_enum::CPP, submission_runner_factory_CPP},
  };
  std::unordered_map<language_enum, decltype(checker_runner_factory_CPP)> checker_runner_factory = {
    {language_enum::CPP, checker_runner_factory_CPP},
  };
  std::unordered_map<language_enum, decltype(generator_runner_factory_CPP)> generator_runner_factory = {
    {language_enum::CPP, generator_runner_factory_CPP},
  };
  std::unordered_map<language_enum, decltype(validator_runner_factory_CPP)> validator_runner_factory = {
    {language_enum::CPP, validator_runner_factory_CPP},
  };
}

super_runner_task* runner_factories::stdio_submission_runner_factory_CPP(
    const std::string& submission_id,
    const std::string& exec_path,
    const std::string& input_path,
    const std::string& output_path,
    float run_time_limit,
    long long run_memory_limit,
    uint8_t priority)
{
    if (submission_id.empty() || exec_path.empty() || input_path.empty() || output_path.empty())
    {
        return nullptr;
    }

    super_runner_task* created = new super_runner_task(
        exec_path,
        input_path,
        output_path,
        "/dev/null",
        run_time_limit,
        run_memory_limit,
        {},
        {},
        {exec_path});

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
    uint8_t priority)
{
    if (submission_id.empty() || exec_path.empty() || input_path.empty() || output_path.empty())
    {
        return nullptr;
    }

    super_runner_task* created = new super_runner_task(
        exec_path,
        "/dev/null",
        "/dev/null",
        "/dev/null",
        run_time_limit,
        run_memory_limit,
        {input_path},
        {output_path},
        {exec_path});

    created->priority = priority;
    return created;
}
