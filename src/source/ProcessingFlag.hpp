#pragma once

/**
 * @file ProcessingFlag.hpp
 * @brief Handlers for parsing and validating operation-specific CLI flags.
 * @details Each function processes a group of related options (color, copy, mirror, split),
 *          validates input formats, updates the argument structure, and sets tracking flags.
 */

#include "CommandLineInterface.hpp" // Для struct argument и error_marker_t
#include "Types.hpp"
#include <string>

/**
 * @brief Processes command-line flags related to color replacement.
 * @details Handles '-c' (activates operation), '-O' (old color), and '-N' (new color).
 *          Validates RGB format and updates tracking flags (has_old_color, has_new_color).
 *
 * @param[in] short_opt The short option character ('c', 'O', or 'N').
 * @param[in] optarg    Pointer to the option argument string (nullptr if no argument).
 * @param[out] arguments Reference to the argument structure to update.
 * @return error_marker_t ERR_OK on success, or an error code on validation failure.
 */
error_marker_t processing_color_flags(char short_opt, const char *optarg, struct argument &arguments);

/**
 * @brief Processes command-line flags related to region copying.
 * @details Handles '-b' (activates operation), '-d' (destination), and delegates
 *          coordinate parsing ('-l', '-r') to internal helpers.
 *          Updates tracking flags (has_dest_left_up, has_left_up, has_right_down).
 *
 * @param[in] short_opt The short option character ('b', 'd', 'l', or 'r').
 * @param[in] optarg    Pointer to the option argument string.
 * @param[out] arguments Reference to the argument structure to update.
 * @return error_marker_t ERR_OK on success, or an error code on validation failure.
 */
error_marker_t processing_copy_flags(char short_opt, const char *optarg, struct argument &arguments);

/**
 * @brief Processes command-line flags related to region mirroring.
 * @details Handles '-m' (activates operation), '-a' (axis validation: 'x' or 'y'),
 *          and delegates coordinate parsing ('-l', '-r') to internal helpers.
 *          Updates tracking flags (has_axis, has_left_up, has_right_down).
 *
 * @param[in] short_opt The short option character ('m', 'a', 'l', or 'r').
 * @param[in] optarg    Pointer to the option argument string.
 * @param[out] arguments Reference to the argument structure to update.
 * @return error_marker_t ERR_OK on success, or an error code on validation failure.
 */
error_marker_t processing_mirror_flag(char short_opt, const char *optarg, struct argument &arguments);

/**
 * @brief Processes command-line flags related to image splitting/grid lines.
 * @details Handles '-s' (activates operation), '-t' (thickness), '-C' (line color),
 *          and delegates numeric ('-x', '-y') and coordinate parsing.
 *          Updates tracking flags (has_thickness, has_color_split, has_number_x, has_number_y).
 *
 * @param[in] short_opt The short option character ('s', 't', 'C', 'x', or 'y').
 * @param[in] optarg    Pointer to the option argument string.
 * @param[out] arguments Reference to the argument structure to update.
 * @return error_marker_t ERR_OK on success, or an error code on validation failure.
 */
error_marker_t processing_split_flag(char short_opt, const char *optarg, struct argument &arguments);