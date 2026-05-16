#pragma once
#include "Types.hpp"

template <typename T> constexpr T clamp(T value, T min_val, T max_val)
{
    return (value < min_val) ? min_val : (value > max_val) ? max_val : value;
}

inline Coordinate clamp_to_image(const Coordinate &coord, int img_width, int img_height)
{
    return Coordinate(clamp(coord.x, 0, img_width - 1), clamp(coord.y, 0, img_height - 1));
}