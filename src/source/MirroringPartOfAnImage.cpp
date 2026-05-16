#include "BmpImage.hpp"
#include <cmath>

void swapPixel(Pixel &first, Pixel &second)
{
    Pixel tmp = first;
    first = second;
    second = tmp;
}

void BmpImage::mirror_image(std::string &axis, Coordinate &left_up, Coordinate &right_down)
{
    int height = abs(m_info_header.height);
    int width = m_info_header.width;

    int x1 = left_up.x;
    int y1 = left_up.y;
    int x2 = right_down.x - 1;
    int y2 = right_down.y - 1;

    if (x1 > x2)
        std::swap(x1, x2);

    if (y1 > y2)
        std::swap(y1, y2);

    x1 = std::max(0, x1);
    y1 = std::max(0, y1);

    x2 = std::min(width - 1, x2);
    y2 = std::min(height - 1, y2);

    if (axis == "x") {
        for (int y = y1; y <= y2; ++y) {
            int row = height - 1 - y;

            for (int x = x1; x <= x1 + (x2 - x1) / 2; ++x) {
                int mirror_x = x2 - (x - x1);

                swapPixel(arr_pixels[row][x], arr_pixels[row][mirror_x]);
            }
        }
    } else if (axis == "y") {
        for (int y = y1; y <= y1 + (y2 - y1) / 2; ++y) {
            int mirror_y = y2 - (y - y1);

            int row1 = height - 1 - y;
            int row2 = height - 1 - mirror_y;

            for (int x = x1; x <= x2; ++x) {
                swapPixel(arr_pixels[row1][x], arr_pixels[row2][x]);
            }
        }
    }
}