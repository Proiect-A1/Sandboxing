#pragma once 

#include <Singletoni/logger.h>

#define handle_error(ret_code , ...) { LOG_FATAL(std::string(__VA_ARGS__)); exit(ret_code); }

#include "json.hpp"
using json = nlohmann::json;

