#pragma once

/**
 * @file ArgumentParser.hpp
 * @brief Utility functions for parsing and validating CLI arguments.
 */

#include "Types.hpp"
#include <string>
#include <utility>

/**
 * @brief Checks if a color component value is within the valid range [0, 255].
 *
 * @param[in] values The color component value to validate.
 * @return true If the value is within the permissible range.
 * @return false If the value is outside the permissible range.
 */

bool check_range_component(int values);

/**
 * @brief Checks if a string represents a valid integer (positive or negative).
 *
 * @param[in] values The string to validate.
 * @return true If the string contains only digits, optionally prefixed by a single '-'.
 * @return false If the format is invalid (e.g., "123-3", "-", "abc", "").
 */
bool check_valid_value(const std::string &values);

/**
 * @brief Parses a string into a Coordinate object.
 *
 * @details Validates the input for the "x.y" format. If parsing fails or
 *          the format is invalid, returns a default Coordinate and an error status.
 *
 * @param[in] str String containing coordinates in "x.y" format.
 * @return std::pair<Pixel , error_marker_t> A pair where:
 *         - first: Parsed Coordinate (0.0 if invalid)
 *         - second: ERR_OK on success, ERR_INCORRECTARG on failure
 */
std::pair<Coordinate, error_marker_t> getCoordinate(const std::string &str);

/**
 * @brief Parses a string into a Pixel object.
 *
 * @details Validates the input for the "r.g.b" format. If parsing fails or
 *          the format is invalid, returns a default Pixel and an error status.
 *
 * @param[in] str String containing Pixel in "r.g.b" format.
 * @return std::pair<Coordinate, error_marker_t> A pair where:
 *         - first: Parsed Pixel (0.0.0 if invalid)
 *         - second: ERR_OK on success, ERR_INCORRECTARG on failure
 */
std::pair<Pixel, error_marker_t> getPixel(const std::string &str);