#include "BmpImage.hpp"
#include <cmath>

void BmpImage::copy_image(Coordinate& left_up,Coordinate& right_down, Coordinate& dest_left_up){
  int cntRows = right_down.y - left_up.y;
  int cntColumns = right_down.x - left_up.x;

  std::vector<std::vector<Pixel>> copy(cntRows,std::vector<Pixel>(cntColumns));

  int height = abs(m_info_header.height);
  int width = m_info_header.width;

  for (int y = left_up.y; y < left_up.y + cntRows;++y){
    int current = height  - 1 - y;
    for (int x = left_up.x; x < left_up.x + cntColumns;++x){
      if (y >= 0 && y < height && x >= 0 && x < width){
        copy[y - left_up.y][x - left_up.x] = arr_pixels[current][x];      
      }
    }
  }

  for (int y = dest_left_up.y; y < dest_left_up.y + cntRows;++y){
    int current = height  - 1 - y;
    for (int x = dest_left_up.x; x < dest_left_up.x + cntColumns;++x){
      if (y >= 0 && y < height && x >= 0 && x < width){
        arr_pixels[current][x] = copy[y - dest_left_up.y][x - dest_left_up.x];
      }
    }
  }
}

