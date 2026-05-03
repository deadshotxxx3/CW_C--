#pragma once
#include <exception>
#include <vector>
#include <cstdint>
#include <string>
#include "Error.hpp"

#pragma pack(push,1)

struct BitmapFileHeader{
    uint16_t signature;
    uint32_t filesize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t pixelArrOffset;
};

struct BitmapInfoHeader{
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t numColors;
    uint32_t importantColorCount;
};

struct Pixel{
    uint8_t b;
    uint8_t g;
    uint8_t r;

    Pixel() : b(0), g(0), r(0) {}
    Pixel(uint8_t r,uint8_t g, uint8_t b): b(b),g(g),r(r){}
};

#pragma pack(pop)

struct Coordinate{
  int x;
  int y;

  Coordinate(): x(0), y(0){}
  Coordinate(int x,int y): x(x),y(y){}
};

class BmpImage{
private:
    BitmapFileHeader m_file_header;
    BitmapInfoHeader m_info_header;
    std::vector<std::vector<Pixel>> arr_pixels;
public:
    BmpImage() = default;

    int getRowStride() const;
    BMPerror readBmp(const std::string& filename);
    BMPerror saveBmp(const std::string& filename);

    const BitmapInfoHeader& getInfoHeader() const { return m_info_header; }
    const BitmapFileHeader& getFileHeader() const { return m_file_header; };
    void print_info() const;

    void swap_r_and_b();
    void mirror_image(std::string& axis,Coordinate& left_up,Coordinate& right_down);
    void copy_image(Coordinate& left_up,Coordinate& right_down, Coordinate& dest_left_up);
    void color_replace(const Pixel& old_color, const Pixel& new_color);
    void toGray();
    void split_image(int cntLineX, int cntLineY,int thickness, Pixel& colorLine);
    void draw_line(Coordinate& pointA, Coordinate& pointB,int thickness,Pixel& color_line);
};
