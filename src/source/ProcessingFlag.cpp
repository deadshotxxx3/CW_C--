#include "ProcessingFlag.hpp"
#include "ArgumentParser.hpp"

#include <cstring>
#include <iostream>

const constexpr int MAX_CNT_LINE = 100; /**< Maximum allowed number of grid divisions (100). */

/**
 * @internal
 * @brief Parses and validates coordinate options ('-l' and '-r').
 *
 * @details Delegates string parsing to getCoordinate(). Validates the "x.y" format,
 *          updates the corresponding field in the argument structure, and sets the
 *          appropriate tracking flag (has_left_up / has_right_down).
 *          Returns immediately on parsing failure without modifying other fields.
 *
 * @param[in]  short_opt Option character to process ('l' or 'r').
 * @param[in]  optarg    Raw command-line argument string.
 * @param[out] arguments Reference to the argument structure to update.
 * @return error_marker_t ERR_OK on success, or ERR_INCORRECTARG if format is invalid.
 */
static error_marker_t process_coordinate_flag(char short_opt, const char *optarg, struct argument &arguments)
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

/**
 * @internal
 * @brief Parses and validates grid division counts ('-x' and '-y').
 *
 * @details Converts the argument string to an integer and validates the range (2 to MAX_CNT_LINE).
 *          Updates number_x/number_y fields and sets the corresponding tracking flag.
 *          Handles std::stoi exceptions and range violations with descriptive error messages.
 *
 * @param[in]  short_opt Option character to process ('x' or 'y').
 * @param[in]  optarg    Raw command-line argument string.
 * @param[out] arguments Reference to the argument structure to update.
 * @return error_marker_t ERR_OK on success, or ERR_INCORRECTARG if out of range or not a number.
 */
static error_marker_t processing_number_line_flag(char short_opt, const char *optarg,
                                                  struct argument &arguments)
{
    if (short_opt == 'x') {
        try {
            int value = std::stoi(optarg);
            if (value <= 1) {
                std::cerr << "Error: number_x must be greater than 1\n";
                return error_marker_t::ERR_INCORRECTARG;
            }
            if (value > MAX_CNT_LINE) {
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
            if (value > MAX_CNT_LINE) {
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