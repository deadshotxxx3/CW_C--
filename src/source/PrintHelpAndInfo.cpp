#include "PrintHelpAndInfo.hpp"
#include "BmpImage.hpp"
#include <iostream>

void print_help()
{
    std::cout << "Course work for option 5.2, created by Andrei Pushin\n";
    std::cout << "Usage: program [OPTIONS]\n";
    std::cout << "\nBasic options:\n";
    std::cout << "  -i, --input FILE            Input BMP file\n";
    std::cout << "  -o, --output FILE           Output BMP file\n";
    std::cout << "  -I, --info                  Show image info\n";
    std::cout << "  -h, --help                  Show this help message\n";
    std::cout << "\nOperations (choose one):\n";
    std::cout << "  -m, --mirror                Mirror a region of the image\n";
    std::cout << "    -a, --axis VALUE          Axis for mirroring: 'x' or 'y'\n";
    std::cout << "    -l, --left_up X.Y         Top-left corner of source region\n";
    std::cout << "    -r, --right_down X.Y      Bottom-right corner of source region\n";
    std::cout << "\n  -b, --copy                  Copy a region to another position\n";
    std::cout << "    -l, --left_up X.Y         Source top-left corner\n";
    std::cout << "    -r, --right_down X.Y      Source bottom-right corner\n";
    std::cout << "    -d, --dest_left_up X.Y    Destination top-left corner\n";
    std::cout << "\n  -c, --color_replace         Replace one color with another\n";
    std::cout << "    -O, --old_color R.G.B     Color to replace (e.g., 255.0.0)\n";
    std::cout << "    -N, --new_color R.G.B     New color (e.g., 0.255.0)\n";
    std::cout << "\n  -s, --split                 Split image into grid with lines\n";
    std::cout << "    -x, --number_x N          Number of columns (>1)\n";
    std::cout << "    -y, --number_y N          Number of rows (>1)\n";
    std::cout << "    -t, --thickness N         Line thickness in pixels (>0)\n";
    std::cout << "    -C, --color R.G.B         Line color (e.g., 0.0.0 for black)\n";
    std::cout << "\nNotes:\n";
    std::cout << "  • Coordinates format: X.Y where X and Y are floating-point numbers\n";
    std::cout << "  • Color format: R.G.B where each component is 0–255\n";
    std::cout << "  • Only one operation can be used at a time\n";
    std::cout << std::endl;
}

void BmpImage::print_info() const
{
    std::cout << "Image Information:\n";
    std::cout << "  File Size: " << m_file_header.filesize << " bytes\n";
    std::cout << "  Width: " << m_info_header.width << " pixels\n";
    std::cout << "  Height: " << m_info_header.height << " pixels\n";
    std::cout << "  Bits per Pixel: " << m_info_header.bitsPerPixel << "\n";
    std::cout << "  Compression: " << m_info_header.compression << "\n";
    std::cout << "  Image Size: " << m_info_header.imageSize << " bytes\n";
    std::cout << "  Colors: " << m_info_header.numColors << std::endl;
}