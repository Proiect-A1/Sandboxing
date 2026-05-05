#ifndef RUNNER_FACTORIES_H
#define RUNNER_FACTORIES_H

#include <cstdint>
#include <memory>
#include <string>

#include <cstdio>
#include <cstdlib>
#include <string>
#include <unordered_map>

#include <Enums/language_enum.h>
#include <Tasks/task.h>
#include <Utilities/architecture_utilities.h>
#include <Utilities/general_utilities.h>
#include <Tasks/super_runner_task.hpp>

namespace runner_factories{
  super_runner_task* stdio_submission_runner_factory_CPP(
      const std::string& submission_id,
      const std::string& exec_path,
      const std::string& input_path,
      const std::string& output_path,
      float run_time_limit,
      long long run_memory_limit,
      uint8_t priority = 0);

  super_runner_task* submission_runner_factory_CPP(
    const std::string& submission_id,
    const std::string& exec_path,
    const std::string& input_path,
    const std::string& output_path,
    float run_time_limit,
    long long run_memory_limit,
    uint8_t priority = 0);
  super_runner_task* checker_runner_factory_CPP(
    const std::string& submission_id,
    const std::string& exec_path,
    const std::string& output_path,
    const std::string& message_path,
    const std::string& submission_input_path,
    const std::string& submission_output_path,
    const std::string& correct_output_path,
    const std::string& source_path,
    const std::vector<std::string>& arguments,
    uint8_t priority = 0);
  super_runner_task* generator_runner_factory_CPP(
    const std::string& submission_id,
    const std::string& exec_path,
    const std::string& output_path,
    const std::string& message_path,
    const std::vector<std::string>& arguments,
    uint8_t priority = 0);
  super_runner_task* validator_runner_factory_CPP(
    const std::string& submission_id,
    const std::string& exec_path,
    const std::string& input_path,
    const std::string& output_path,
    const std::string& message_path,
    const std::vector<std::string>& arguments,
    uint8_t priority = 0);

  extern std::unordered_map<language_enum, decltype(&stdio_submission_runner_factory_CPP)> stdio_submission_runner_factory;
  extern std::unordered_map<language_enum, decltype(&submission_runner_factory_CPP)> submission_runner_factory;
  extern std::unordered_map<language_enum, decltype(&checker_runner_factory_CPP)> checker_runner_factory;
  extern std::unordered_map<language_enum, decltype(&generator_runner_factory_CPP)> generator_runner_factory;
  extern std::unordered_map<language_enum, decltype(&validator_runner_factory_CPP)> validator_runner_factory;
}






#endif
