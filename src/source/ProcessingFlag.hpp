#pragma once

#include "CommandLineInterface.hpp"
#include "Types.hpp"
#include <string>

error_marker_t processing_color_flags(char short_opt, const char *optarg, struct argument &arguments);
error_marker_t processing_copy_flags(char short_opt, const char *optarg, struct argument &arguments);
error_marker_t processing_mirror_flag(char short_opt, const char *optarg, struct argument &arguments);
error_marker_t processing_split_flag(char short_opt, const char *optarg, struct argument &arguments);