#include "BmpImage.hpp"
#include <algorithm>
#include <iostream>

void BmpImage::color_replace(const Pixel &old_color, const Pixel &new_color)
{
    for (auto &row : m_arr_pixel) {
        for (auto &pixel : row) {
            if (pixel == old_color)
                pixel = new_color;
        }
    }
}