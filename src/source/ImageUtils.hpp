#pragma once

/**
 * @file ImageUtils.hpp
 * @brief Geometric and mathematical utilities for image processing.
 * @details Contains clamping, coordinate validation, and helper math functions.
 */

#include "Types.hpp"

/**
 * @brief Clamps a value between a minimum and maximum boundary.
 *
 * @tparam T Any comparable type (arithmetic, enum, or custom class with < and >).
 * @param[in] value The value to clamp.
 * @param[in] min_val The lower bound.
 * @param[in] max_val The upper bound.
 * @return The clamped value: min_val if value < min_val, max_val if value > max_val, otherwise value.
 */
template <typename T> constexpr T clamp(T value, T min_val, T max_val)
{
    return (value < min_val) ? min_val : (value > max_val) ? max_val : value;
}

/**
 * @brief Clamps coordinate values to fit within image boundaries.
 *
 * @param[in] coord The original coordinate to clamp.
 * @param[in] img_width The width of the image in pixels.
 * @param[in] img_height The height of the image in pixels.
 * @return A new Coordinate with x and y values restricted to [0, width-1] and [0, height-1].
 */
inline Coordinate clamp_to_image(const Coordinate &coord, int img_width, int img_height)
{
    return Coordinate(clamp(coord.x, 0, img_width - 1), clamp(coord.y, 0, img_height - 1));
}