#include "BmpImage.hpp"
#include <cmath>

void swapPixel(Pixel& first, Pixel& second){
  Pixel tmp = first;
  first = second;
  second = tmp;
}
  
void BmpImage::mirror_image(std::string& axis, Coordinate& left_up, Coordinate& right_down){
    int cntRows = right_down.y - left_up.y;
    int cntColumns = right_down.x - left_up.x;

    int height = abs(m_info_header.height);

    if (axis == "y"){
        for (int y = left_up.y; y < left_up.y + cntRows; ++y){
            int true_y = height - y - 1;
            for (int x = left_up.x; x < left_up.x + cntColumns / 2; ++x){
                int mirror_x = left_up.x + (cntColumns - 1) - (x - left_up.x);
                swapPixel(arr_pixels[true_y][x],arr_pixels[true_y][mirror_x]);
            }
        }
    }
    if (axis == "x"){
      for (int x = left_up.x; x < left_up.x + cntColumns;++x){
        for (int y = left_up.y; y < left_up.y + cntRows / 2;++y){
          int mirror_y = left_up.y + (cntRows - 1) - (y - left_up.y);
          int true_y = height - 1 - y;
          int true_mirror_y = height - mirror_y - 1;
          swapPixel(arr_pixels[true_y][x],arr_pixels[true_mirror_y][x]);
        }
      }
    }
}
