#pragma once 
#define handle_error(ret_code , ...) { fprintf(stderr , __VA_ARGS__); exit(ret_code); }

#include "json.hpp"
using json = nlohmann::json;

