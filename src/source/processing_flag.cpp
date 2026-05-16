#include "processing_flag.hpp"
#include <cstring>
#include <iostream>

#define CHECK_RESULT(result, flag_found)                                                                     \
    if (result != error_marker_t::ERR_OK)                                                                    \
        return result;                                                                                       \
    if (flag_found)                                                                                          \
        return error_marker_t::ERR_OK;

error_marker_t process_coordinate_flag(const std::string &opt_name, const char *optarg,
                                       struct argument &arguments, bool &flag_found)
{
    if (opt_name == OptionNames::LEFT_UP) {
        auto result = getCoordinate(optarg);
        if (result.second != error_marker_t::ERR_OK) {
            return result.second;
        }
        arguments.left_up = result.first;
        flag_found = true;
        return error_marker_t::ERR_OK;
    }

    else if (opt_name == OptionNames::RIGHT_DOWN) {
        auto result = getCoordinate(optarg);
        if (result.second != error_marker_t::ERR_OK) {
            return result.second;
        }
        arguments.right_down = result.first;
        flag_found = true;
        return error_marker_t::ERR_OK;
    }

    return error_marker_t::ERR_OK;
}

error_marker_t processing_color_flags(const std::string &opt_name, const char *optarg,
                                      struct argument &arguments, bool &flag_found)
{
    if (opt_name == OptionNames::COLOR_REPLACE) {
        arguments.flag = flags::FLAG_COLOR_REPLACE;
        flag_found = true;
        return error_marker_t::ERR_OK;
    }

    else if (opt_name == OptionNames::OLD_COLOR) {
        auto result = getPixel(optarg);
        if (result.second != error_marker_t::ERR_OK) {
            return result.second;
        }
        arguments.old_color = result.first;
        flag_found = true;
        return error_marker_t::ERR_OK;
    }

    else if (opt_name == OptionNames::NEW_COLOR) {
        auto result = getPixel(optarg);
        if (result.second != error_marker_t::ERR_OK) {
            return result.second;
        }
        arguments.new_color = result.first;
        flag_found = true;
        return error_marker_t::ERR_OK;
    }

    return error_marker_t::ERR_OK;
}

error_marker_t processing_copy_flags(const std::string &opt_name, const char *optarg,
                                     struct argument &arguments, bool &flag_found)
{
    if (opt_name == OptionNames::COPY) {
        arguments.flag = flags::FLAG_COPY;
        flag_found = true;
        return error_marker_t::ERR_OK;
    }

    else if (opt_name == OptionNames::DEST_LEFT_UP) {
        auto result = getCoordinate(optarg);
        if (result.second != error_marker_t::ERR_OK) {
            return result.second;
        }
        arguments.dest_left_up = result.first;
        flag_found = true;
        return error_marker_t::ERR_OK;
    }

    error_marker_t coord_result = process_coordinate_flag(opt_name, optarg, arguments, flag_found);
    if (coord_result != error_marker_t::ERR_OK) {
        return coord_result;
    }

    return error_marker_t::ERR_OK;
}

error_marker_t processing_mirror_flag(const std::string &opt_name, const char *optarg,
                                      struct argument &arguments, bool &flag_found)
{
    if (opt_name == OptionNames::MIRROR) {
        arguments.flag = flags::FLAG_MIRROR;
        flag_found = true;
        return error_marker_t::ERR_OK;
    }

    else if (opt_name == OptionNames::AXIS) {
        if (strcmp(optarg, "x") != 0 && strcmp(optarg, "y") != 0) {
            std::cerr << "Error: axis must be 'x' or 'y'\n";
            return error_marker_t::ERR_INCORRECTARG;
        }
        arguments.axis = optarg;
        flag_found = true;
        return error_marker_t::ERR_OK;
    }

    error_marker_t coord_result = process_coordinate_flag(opt_name, optarg, arguments, flag_found);
    if (coord_result != error_marker_t::ERR_OK) {
        return coord_result;
    }

    return error_marker_t::ERR_OK;
}

error_marker_t processing_number_line_flag(const std::string &opt_name, const char *optarg,
                                           struct argument &arguments, bool &flag_found)
{
    if (opt_name == OptionNames::NUMBER_X) {
        try {
            int value = std::stoi(optarg);
            if (value <= 1) {
                std::cerr << "Error: number_x must be greater than 1\n";
                return error_marker_t::ERR_INCORRECTARG;
            }
            if (value > 100) {
                std::cerr << "Error: number_x too large\n";
                return error_marker_t::ERR_INCORRECTARG;
            }
            arguments.number_x = value;
        } catch (...) {
            std::cerr << "Error: invalid number for number_x\n";
            return error_marker_t::ERR_INCORRECTARG;
        }
        flag_found = true;
        return error_marker_t::ERR_OK;
    }

    else if (opt_name == OptionNames::NUMBER_Y) {
        try {
            int value = std::stoi(optarg);
            if (value <= 1) {
                std::cerr << "Error: number_y must be greater than 1\n";
                return error_marker_t::ERR_INCORRECTARG;
            }
            if (value > 100) {
                std::cerr << "Error: number_y too large\n";
                return error_marker_t::ERR_INCORRECTARG;
            }
            arguments.number_y = value;
        } catch (...) {
            std::cerr << "Error: invalid number for number_y\n";
            return error_marker_t::ERR_INCORRECTARG;
        }
        flag_found = true;
        return error_marker_t::ERR_OK;
    }
    return error_marker_t::ERR_OK;
}

error_marker_t processing_split_flag(const std::string &opt_name, const char *optarg,
                                     struct argument &arguments, bool &flag_found)
{
    if (opt_name == OptionNames::SPLIT) {
        arguments.flag = flags::FLAG_SPLIT;
        flag_found = true;
        return error_marker_t::ERR_OK;
    }

    else if (opt_name == OptionNames::THICKNESS) {
        try {
            int value = std::stoi(optarg);
            if (value < 0) {
                std::cerr << "Error: thickness must be greater than 0\n";
                return error_marker_t::ERR_INCORRECTARG;
            }
            arguments.thickness = value;
        } catch (...) {
            std::cerr << "Error: invalid number for thickness\n";
            return error_marker_t::ERR_INCORRECTARG;
        }
        flag_found = true;
        return error_marker_t::ERR_OK;
    }

    else if (opt_name == OptionNames::COLOR) {
        auto result = getPixel(optarg);
        if (result.second != error_marker_t::ERR_OK) {
            return result.second;
        }
        arguments.color_split = result.first;
        flag_found = true;
        return error_marker_t::ERR_OK;
    }
    error_marker_t number_line_result = processing_number_line_flag(opt_name, optarg, arguments, flag_found);
    if (number_line_result != error_marker_t::ERR_OK) {
        return number_line_result;
    }
    error_marker_t coord_result = process_coordinate_flag(opt_name, optarg, arguments, flag_found);
    if (coord_result != error_marker_t::ERR_OK) {
        return coord_result;
    }

    return error_marker_t::ERR_OK;
}

error_marker_t process_all_flags(const std::string &opt_name, const char *optarg, struct argument &arguments)
{
    bool flag_found = false;

    error_marker_t result;

    result = processing_color_flags(opt_name, optarg, arguments, flag_found);
    CHECK_RESULT(result, flag_found);

    result = processing_copy_flags(opt_name, optarg, arguments, flag_found);
    CHECK_RESULT(result, flag_found);

    result = processing_mirror_flag(opt_name, optarg, arguments, flag_found);
    CHECK_RESULT(result, flag_found);

    result = processing_split_flag(opt_name, optarg, arguments, flag_found);
    CHECK_RESULT(result, flag_found);

    result = process_coordinate_flag(opt_name, optarg, arguments, flag_found);
    CHECK_RESULT(result, flag_found);

    if (opt_name == OptionNames::INFO) {
        arguments.flag = flags::FLAG_INFO;
        return error_marker_t::ERR_OK;
    }

    return error_marker_t::ERR_INCORRECTARG;
}