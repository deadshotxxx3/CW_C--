#include "BmpImage.hpp"
#include "ImageUtils.hpp"
#include <algorithm>
#include <cmath>

void BmpImage::copy_image(Coordinate &left_up, Coordinate &right_down, Coordinate &dest_left_up)
{
    int width = m_info_header.width;
    int height = std::abs(m_info_header.height);

    left_up = clamp_to_image(left_up, width, height);
    right_down = clamp_to_image(right_down, width, height);

    int cntColumns = right_down.x - left_up.x + 1;
    int cntRows = right_down.y - left_up.y + 1;

    if (cntColumns <= 0 || cntRows <= 0)
        return;

    std::vector<std::vector<Pixel>> buffer(cntRows, std::vector<Pixel>(cntColumns));

    for (int dy = 0; dy < cntRows; ++dy) {
        int src_y = left_up.y + dy;
        if (src_y >= height)
            break;
        int bmp_src_y = height - 1 - src_y;
        for (int dx = 0; dx < cntColumns; ++dx) {
            int src_x = left_up.x + dx;
            if (src_x >= width)
                break;
            buffer[dy][dx] = arr_pixels[bmp_src_y][src_x];
        }
    }

    for (int dy = 0; dy < cntRows; ++dy) {
        int target_y = dest_left_up.y + dy;
        if (target_y < 0 || target_y >= height)
            continue;
        int bmp_tgt_y = height - 1 - target_y;
        for (int dx = 0; dx < cntColumns; ++dx) {
            int target_x = dest_left_up.x + dx;
            if (target_x < 0 || target_x >= width)
                continue;
            arr_pixels[bmp_tgt_y][target_x] = buffer[dy][dx];
        }
    }
}