#include "BmpImage.hpp"
#include <algorithm>
#include <cmath>

void copySourceToBuffer(const std::vector<std::vector<Pixel>> &src, std::vector<std::vector<Pixel>> &dst,
                        int x1, int y1, int x2, int y2, int height, int width)
{
    int rows = y2 - y1;
    int cols = x2 - x1;
    for (int dy = 0; dy < rows; ++dy) {
        int y = y1 + dy;
        if (y < 0 || y >= height)
            continue;
        int src_y = height - 1 - y;
        for (int dx = 0; dx < cols; ++dx) {
            int x = x1 + dx;
            if (x < 0 || x >= width)
                continue;
            dst[dy][dx] = src[src_y][x];
        }
    }
}

void copyBufferToTarget(std::vector<std::vector<Pixel>> &target,
                        const std::vector<std::vector<Pixel>> &buffer, int dest_x, int dest_y, int rows,
                        int cols, int height, int width)
{
    for (int dy = 0; dy < rows; ++dy) {
        int y = dest_y + dy;
        if (y < 0 || y >= height)
            continue;
        int target_y = height - 1 - y;
        for (int dx = 0; dx < cols; ++dx) {
            int x = dest_x + dx;
            if (x < 0 || x >= width)
                continue;
            target[target_y][x] = buffer[dy][dx];
        }
    }
}

void BmpImage::copy_image(Coordinate &left_up, Coordinate &right_down, Coordinate &dest_left_up)
{
    int width = m_info_header.width;
    int height = std::abs(m_info_header.height);

    int x1 = left_up.x;
    int y1 = left_up.y;
    int x2 = right_down.x;
    int y2 = right_down.y;

    int cntColumns = x2 - x1;
    int cntRows = y2 - y1;

    if (cntColumns <= 0 || cntRows <= 0)
        return;

    std::vector<std::vector<Pixel>> buffer(cntRows, std::vector<Pixel>(cntColumns));

    copySourceToBuffer(arr_pixels, buffer, x1, y1, x2, y2, height, width);

    copyBufferToTarget(arr_pixels, buffer, dest_left_up.x, dest_left_up.y, cntRows, cntColumns, height,
                       width);
}