#include "BmpImage.hpp"
#include "ImageUtils.hpp"
#include <cmath>

static void swapPixel(Pixel &first, Pixel &second)
{
    Pixel tmp = first;
    first = second;
    second = tmp;
}

void BmpImage::mirror_image(const std::string &axis, Coordinate &left_up, Coordinate &right_down)
{
    int height = std::abs(m_info_header.height);
    int width = m_info_header.width;

    left_up = clamp_to_image(left_up, width, height);
    right_down = clamp_to_image(right_down, width, height);

    int x_left_up = left_up.x;
    int y_left_up = left_up.y;
    int x_right_down = right_down.x - 1;
    int y_right_down = right_down.y - 1;

    if (axis == "x") {
        for (int y = y_left_up; y <= y_right_down; ++y) {
            int row = height - 1 - y;

            for (int x = x_left_up; x <= x_left_up + (x_right_down - x_left_up) / 2; ++x) {
                int mirror_x = x_right_down - (x - x_left_up);

                swapPixel(m_arr_pixel[row][x], m_arr_pixel[row][mirror_x]);
            }
        }
    } else if (axis == "y") {
        for (int y = y_left_up; y <= y_left_up + (y_right_down - y_left_up) / 2; ++y) {
            int mirror_y = y_right_down - (y - y_left_up);

            int row1 = height - 1 - y;
            int row2 = height - 1 - mirror_y;

            for (int x = x_left_up; x <= x_right_down; ++x) {
                swapPixel(m_arr_pixel[row1][x], m_arr_pixel[row2][x]);
            }
        }
    }
}