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

void BmpImage::swap_r_and_b() {
    for (auto& row : arr_pixels) {
        for (auto& pixel : row) {
            int save = pixel.r;
            std::swap(pixel.r, pixel.g);
            pixel.r = save;
        }
    }
}

void BmpImage::toGray(){
  for (int y = 0; y < m_info_header.height;++y){
    for (int x = 0; x < m_info_header.width;++x){
      int gray = 0.299 * arr_pixels[y][x].r + 0.587 * arr_pixels[y][x].g + 0.114 * arr_pixels[y][x].b;

      arr_pixels[y][x].r = gray;
      arr_pixels[y][x].g = gray;
      arr_pixels[y][x].b = gray;
    }
  }
}
