#include "BmpImage.hpp"
#include <cmath>
#include <fstream>
#include <iostream>

static constexpr uint8_t BYTES_PER_PIXEL = 3;

error_marker_t BmpImage::saveBmp(const std::string &filename)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Cannot write file" << std::endl;
        return error_marker_t::ERR_WRITING;
    }

    int row_stride = getRowStride();
    uint32_t pixel_data_size = row_stride * std::abs(m_info_header.height);

    m_file_header.filesize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + pixel_data_size;

    file.write(reinterpret_cast<const char *>(&m_file_header), sizeof(m_file_header));
    if (!file) {
        std::cerr << "Error: Failed to write BMP file header." << std::endl;
        return error_marker_t::ERR_WRITING;
    }

    file.write(reinterpret_cast<const char *>(&m_info_header), sizeof(m_info_header));
    if (!file) {
        std::cerr << "Error: Failed to write BMP info header." << std::endl;
        return error_marker_t::ERR_WRITING;
    }

    uint32_t H = abs(m_info_header.height);

    for (uint32_t i = 0; i < H; ++i) {
        file.write(reinterpret_cast<const char *>(arr_pixels[i].data()), m_info_header.width * 3);

        for (int j = 0; j < row_stride - m_info_header.width * BYTES_PER_PIXEL; ++j) {
            file.put(0);
        }
    }

    file.close();

    return error_marker_t::ERR_OK;
}