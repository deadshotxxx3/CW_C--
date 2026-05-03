#pragma once
#include <vector>
#include <string>
#include "Error.hpp"
#include "Types.hpp"


class BmpImage{
private:
    BitmapFileHeader m_file_header;
    BitmapInfoHeader m_info_header;
    std::vector<std::vector<Pixel>> arr_pixels;
public:
    BmpImage() = default;

    int getRowStride() const;
    error_marker_t readBmp(const std::string& filename);
    error_marker_t saveBmp(const std::string& filename);

    const BitmapInfoHeader& getInfoHeader() const { return m_info_header; }
    const BitmapFileHeader& getFileHeader() const { return m_file_header; };

    void mirror_image(std::string& axis,Coordinate& left_up,Coordinate& right_down);
    void copy_image(Coordinate& left_up,Coordinate& right_down, Coordinate& dest_left_up);
    void color_replace(const Pixel& old_color, const Pixel& new_color);
    void split_image(int cntLineX, int cntLineY,int thickness, Pixel& colorLine);
    void draw_line(Coordinate& pointA, Coordinate& pointB,int thickness,Pixel& color_line);
    void print_info() const;
};
