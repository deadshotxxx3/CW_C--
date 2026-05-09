#include "BmpImage.hpp"

void replace_color_pixel(Pixel &pixel, Pixel &need_color)
{
    pixel.r = need_color.r;
    pixel.b = need_color.b;
    pixel.g = need_color.g;
}

void BmpImage::draw_line(Coordinate &pointA, Coordinate &pointB, int thickness, Pixel &color_line)
{
    int height = m_info_header.height;
    int width = m_info_header.width;

    if (pointA.x == pointB.x) {
        for (int y = pointA.y; y < pointB.y; ++y) {
            for (int j = 0; j < thickness; ++j) {
                if (height - y >= 0 && pointA.x - j >= 0 && pointA.x - j < width && height - y < height) {
                    replace_color_pixel(arr_pixels[height - y][pointA.x - j], color_line);
                }
            }
        }
    } else if (pointA.y == pointB.y) {
        for (int x = pointA.x; x < pointB.x; ++x) {
            for (int j = 0; j < thickness; ++j) {
                if (height - pointA.y + j >= 0 && x >= 0 && x < width && height - pointA.y + j < height) {
                    replace_color_pixel(arr_pixels[height - pointA.y + j][x], color_line);
                }
            }
        }
    }
}

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

    int new_row_size = ((new_w * 3 + 3) / 4) * 4;
    int new_image_size = new_row_size * new_h;

    m_info_header.width = new_w;
    m_info_header.height = new_h;
    m_info_header.imageSize = new_image_size;
    m_file_header.filesize = 54 + new_image_size;

    arr_pixels = std::move(new_pixels);
}