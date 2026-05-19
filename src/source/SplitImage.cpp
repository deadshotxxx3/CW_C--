#include "BmpImage.hpp"

void BmpImage::split_image(int cntLineX, int cntLineY, int thickness, Pixel &colorLine)
{
    const int orig_w = m_info_header.width;
    const int orig_h = m_info_header.height;

    constexpr double COORD_ROUNDING_BIAS = 0.5;
    constexpr int SEPARATOR_GAP_OFFSET = 1;

    int new_w = orig_w + (cntLineX - SEPARATOR_GAP_OFFSET) * thickness;
    int new_h = orig_h + (cntLineY - SEPARATOR_GAP_OFFSET) * thickness;

    std::vector<std::vector<Pixel>> new_pixels(new_h, std::vector<Pixel>(new_w, colorLine));

    for (int y = 0; y < orig_h; ++y) {
        int blockY = static_cast<int>((y + COORD_ROUNDING_BIAS) * cntLineY / orig_h);
        if (blockY >= cntLineY)
            blockY = cntLineY - 1;

        int newY = y + blockY * thickness;

        for (int x = 0; x < orig_w; ++x) {
            int blockX = static_cast<int>((x + COORD_ROUNDING_BIAS) * cntLineX / orig_w);
            if (blockX >= cntLineX)
                blockX = cntLineX - 1;

            int newX = x + blockX * thickness;
            new_pixels[newY][newX] = arr_pixels[y][x];
        }
    }

    updateDimensions(new_w, new_h);

    arr_pixels = std::move(new_pixels);
}