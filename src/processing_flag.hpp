#pragma once

#include "BmpImage.hpp"
#include <string>
#include "CommandLineInterface.hpp"

int processing_split_flag(const std::string& opt_name,const char* optarg, struct argument& arguments);
int processing_mirror_flag(const std::string& opt_name,const char* optarg, struct argument& arguments);
int processing_copy_flags(const std::string& opt_name, const char* optarg, struct argument& arguments);
int processing_color_flags(const std::string& opt_name, const char* optarg, struct argument& arguments);

int process_all_flags(const std::string& opt_name, const char* optarg, struct argument& arguments);