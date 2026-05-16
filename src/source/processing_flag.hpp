#pragma once

#include "CommandLineInterface.hpp"
#include "Types.hpp"
#include <string>

error_marker_t process_all_flags(const std::string &opt_name, const char *optarg, struct argument &arguments);