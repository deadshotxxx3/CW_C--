#include "BmpImage.hpp"

void BmpImage::split_image(int cntLineX, int cntLineY, int thickness, Pixel &colorLine)
{
    int orig_w = m_info_header.width;
    int orig_h = m_info_header.height;

    int new_w = orig_w + (cntLineX - 1) * thickness;
    int new_h = orig_h + (cntLineY - 1) * thickness;

    std::vector<std::vector<Pixel>> new_pixels(new_h, std::vector<Pixel>(new_w, colorLine));

    for (int y = 0; y < orig_h; ++y) {
        int blockY = (int)((y + 0.5) * cntLineY / orig_h);

        if (blockY >= cntLineY)
            blockY = cntLineY - 1;

        int newY = y + blockY * thickness;

        for (int x = 0; x < orig_w; ++x) {
            int blockX = (int)((x + 0.5) * cntLineX / orig_w);

            if (blockX >= cntLineX)
                blockX = cntLineX - 1;

            int newX = x + blockX * thickness;

            new_pixels[newY][newX] = arr_pixels[y][x];
        }
    }

    updateDimensions(new_w, new_h);

    arr_pixels = std::move(new_pixels);
}