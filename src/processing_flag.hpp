#pragma once

#include <string>
#include "CommandLineInterface.hpp"
#include "Types.hpp"

error_marker_t process_all_flags(const std::string& opt_name, const char* optarg, struct argument& arguments);