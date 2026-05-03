#include "BmpImage.hpp"
#include <algorithm>

void BmpImage::color_replace(const Pixel& old_color, const Pixel& new_color){
  for (auto& row : arr_pixels){
    for (auto& pixel : row){
      if (pixel.r == old_color.r &&
          pixel.g == old_color.g &&
          pixel.b == old_color.b) pixel = new_color;
    }
  }
}