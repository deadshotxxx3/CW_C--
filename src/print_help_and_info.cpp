#include "print_help_and_info.hpp"
#include <iostream>
#include "BmpImage.hpp"

void print_help(){
    std::cout << "Course work for option 5.2, created by Andrei Pushin\n";
    std::cout << "Usage: program [OPTIONS]\n";
    std::cout << "Options:\n";
    std::cout << "  -i, --input FILE          Input BMP file\n";
    std::cout << "  -o, --output FILE         Output BMP file\n";
    std::cout << "  -h, --help                Show this help message\n";
    std::cout << "\nOperations:\n";
    std::cout << "  --mirror                  Mirror a region\n";
    std::cout << "    --axis VALUE            Axis for mirroring (x/y)\n";
    std::cout << "    --left_up X.Y           Top-left corner (x.y)\n";
    std::cout << "    --right_down X.Y        Bottom-right corner (x.y)\n";
    std::cout << "\n  --copy                    Copy a region\n";
    std::cout << "    --left_up X.Y           Source top-left corner (x.y)\n";
    std::cout << "    --right_down X.Y        Source bottom-right corner (x.y)\n";
    std::cout << "    --dest_left_up X.Y      Destination top-left corner (x.y)\n";
    std::cout << "\n  --color\n";
    std::cout << "    --old_color R.G.B       Old color (example: 255.0.0 for red)\n";
    std::cout << "    --new_color R.G.B       New color (example: 0.255.0 for green)\n";
    std::cout << "\n  --split                   Split image into N*M parts\n";
    std::cout << "    --number_x N            Number of parts along X (>1)\n";
    std::cout << "    --number_y N            Number of parts along Y (>1)\n";
    std::cout << "    --thickness N           Line thickness (>0)\n";
    std::cout << "    --color R.G.B           Line color (e.g., 255.0.0)\n";
    std::cout << std::endl;
}

void BmpImage::print_info() const {
    std::cout << "Image Information:\n";
    std::cout << "  File Size: " << m_file_header.filesize << " bytes\n";
    std::cout << "  Width: " << m_info_header.width << " pixels\n";
    std::cout << "  Height: " << m_info_header.height << " pixels\n";
    std::cout << "  Bits per Pixel: " << m_info_header.bitsPerPixel << "\n";
    std::cout << "  Compression: " << m_info_header.compression << "\n";
    std::cout << "  Image Size: " << m_info_header.imageSize << " bytes\n";
    std::cout << "  Colors: " << m_info_header.numColors << "\n";
    std::cout << std::endl;
}