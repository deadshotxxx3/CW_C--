#include "ProcessingFlag.hpp"
#include <cstring>
#include <iostream>

error_marker_t process_coordinate_flag(char short_opt, const char *optarg, struct argument &arguments)
{
    if (short_opt == 'l') {
        auto result = getCoordinate(optarg);
        if (result.second != error_marker_t::ERR_OK) {
            return result.second;
        }
        arguments.left_up = result.first;
        arguments.has_left_up = true;
        return error_marker_t::ERR_OK;
    } else if (short_opt == 'r') {
        auto result = getCoordinate(optarg);
        if (result.second != error_marker_t::ERR_OK) {
            return result.second;
        }
        arguments.right_down = result.first;
        arguments.has_right_down = true;
        return error_marker_t::ERR_OK;
    }
    return error_marker_t::ERR_OK;
}

error_marker_t processing_color_flags(char short_opt, const char *optarg, struct argument &arguments)
{
    if (short_opt == 'c') {
        arguments.flag = flags::FLAG_COLOR_REPLACE;
        return error_marker_t::ERR_OK;
    } else if (short_opt == 'O') {
        auto result = getPixel(optarg);
        if (result.second != error_marker_t::ERR_OK) {
            return result.second;
        }
        arguments.old_color = result.first;
        arguments.has_old_color = true;
        return error_marker_t::ERR_OK;
    } else if (short_opt == 'N') {
        auto result = getPixel(optarg);
        if (result.second != error_marker_t::ERR_OK) {
            return result.second;
        }
        arguments.new_color = result.first;
        arguments.has_new_color = true;
        return error_marker_t::ERR_OK;
    }
    return error_marker_t::ERR_OK;
}

error_marker_t processing_copy_flags(char short_opt, const char *optarg, struct argument &arguments)
{
    if (short_opt == 'b') {
        arguments.flag = flags::FLAG_COPY;
        return error_marker_t::ERR_OK;
    } else if (short_opt == 'd') {
        auto result = getCoordinate(optarg);
        if (result.second != error_marker_t::ERR_OK) {
            return result.second;
        }
        arguments.dest_left_up = result.first;
        arguments.has_dest_left_up = true;
        return error_marker_t::ERR_OK;
    }
    error_marker_t coord_result = process_coordinate_flag(short_opt, optarg, arguments);
    if (coord_result != error_marker_t::ERR_OK) {
        return coord_result;
    }
    return error_marker_t::ERR_OK;
}

error_marker_t processing_mirror_flag(char short_opt, const char *optarg, struct argument &arguments)
{
    if (short_opt == 'm') {
        arguments.flag = flags::FLAG_MIRROR;
        return error_marker_t::ERR_OK;
    } else if (short_opt == 'a') {
        if (strcmp(optarg, "x") != 0 && strcmp(optarg, "y") != 0) {
            std::cerr << "Error: axis must be 'x' or 'y'\n";
            return error_marker_t::ERR_INCORRECTARG;
        }
        arguments.axis = optarg;
        arguments.has_axis = true;
        return error_marker_t::ERR_OK;
    }
    error_marker_t coord_result = process_coordinate_flag(short_opt, optarg, arguments);
    if (coord_result != error_marker_t::ERR_OK) {
        return coord_result;
    }
    return error_marker_t::ERR_OK;
}

error_marker_t processing_number_line_flag(char short_opt, const char *optarg, struct argument &arguments)
{
    if (short_opt == 'x') {
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
            arguments.has_number_x = true;
        } catch (...) {
            std::cerr << "Error: invalid number for number_x\n";
            return error_marker_t::ERR_INCORRECTARG;
        }
        return error_marker_t::ERR_OK;
    } else if (short_opt == 'y') {
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
            arguments.has_number_y = true;
        } catch (...) {
            std::cerr << "Error: invalid number for number_y\n";
            return error_marker_t::ERR_INCORRECTARG;
        }
        return error_marker_t::ERR_OK;
    }
    return error_marker_t::ERR_OK;
}

error_marker_t processing_split_flag(char short_opt, const char *optarg, struct argument &arguments)
{
    if (short_opt == 's') {
        arguments.flag = flags::FLAG_SPLIT;
        return error_marker_t::ERR_OK;
    } else if (short_opt == 't') {
        try {
            int value = std::stoi(optarg);
            if (value <= 0) {
                std::cerr << "Error: thickness must be greater than 0\n";
                return error_marker_t::ERR_INCORRECTARG;
            }
            arguments.thickness = value;
            arguments.has_thickness = true;
        } catch (...) {
            std::cerr << "Error: invalid number for thickness\n";
            return error_marker_t::ERR_INCORRECTARG;
        }
        return error_marker_t::ERR_OK;
    } else if (short_opt == 'C') {
        auto result = getPixel(optarg);
        if (result.second != error_marker_t::ERR_OK) {
            return result.second;
        }
        arguments.color_split = result.first;
        arguments.has_color_split = true;
        return error_marker_t::ERR_OK;
    }
    error_marker_t number_line_result = processing_number_line_flag(short_opt, optarg, arguments);
    if (number_line_result != error_marker_t::ERR_OK) {
        return number_line_result;
    }
    error_marker_t coord_result = process_coordinate_flag(short_opt, optarg, arguments);
    if (coord_result != error_marker_t::ERR_OK) {
        return coord_result;
    }
    return error_marker_t::ERR_OK;
}