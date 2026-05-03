#include "BmpImage.hpp"

void replace_color_pixel(Pixel& pixel,Pixel& need_color){
  pixel.r = need_color.r;
  pixel.b = need_color.b;
  pixel.g = need_color.g;
}

void BmpImage::draw_line(Coordinate& pointA, Coordinate& pointB,int thickness,Pixel& color_line){
  int height = m_info_header.height;
  int width = m_info_header.width;

  if (pointA.x == pointB.x){
    for (int y = pointA.y; y < pointB.y;++y){
      for (int j = 0; j < thickness;++j){
        if (height - y >= 0 && pointA.x - j >= 0 && pointA.x - j < width
            && height - y < height){
            replace_color_pixel(arr_pixels[height - y][pointA.x - j],color_line);
        }
      }
    }
  }
  else if (pointA.y == pointB.y){
    for (int x = pointA.x; x < pointB.x;++x){
      for (int j = 0; j < thickness;++j){
        if (height - pointA.y + j >= 0 && x >=0 
          && x < width && height - pointA.y + j < height){
            replace_color_pixel(arr_pixels[height - pointA.y + j][x],color_line);
          }
      }
    }
  }
}

void BmpImage::split_image(int cntLineX, int cntLineY,int thickness, Pixel& colorLine){
  int width = m_info_header.width;
  int height = m_info_header.height;
  for (int i = 1; i < cntLineX;++i){
    Coordinate pixelA(width * i / cntLineX,0);
    Coordinate pixelB(width * i / cntLineX,height);
    draw_line(pixelA,pixelB,thickness,colorLine);
  }
  for (int i = 1; i < cntLineY;++i){
    Coordinate pixelA(0, height * i / cntLineY);
    Coordinate pixelB(width,height * i / cntLineY);
    draw_line(pixelA,pixelB,thickness,colorLine);
  }
}

