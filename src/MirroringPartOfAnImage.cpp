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

    int x1 = left_up.x;
    int y1 = left_up.y;
    int x2 = right_down.x;
    int y2 = right_down.y;

    if (axis == "x") {
        for (int y = y1; y <= y2; ++y) {
            int true_y = height - 1 - y;

            for (int x = x1; x <= (x1 + x2) / 2; ++x) {
                int mirror_x = x2 - (x - x1);

                swapPixel(arr_pixels[true_y][x], arr_pixels[true_y][mirror_x]);
            }
        }
    }

    if (axis == "y") {
        for (int x = x1; x <= x2; ++x) {
            for (int y = y1; y <= (y1 + y2) / 2; ++y) {
                int mirror_y = y2 - (y - y1);

                int true_y = height - 1 - y;
                int true_mirror_y = height - 1 - mirror_y;

                swapPixel(arr_pixels[true_y][x], arr_pixels[true_mirror_y][x]);
            }
        }
    }
}