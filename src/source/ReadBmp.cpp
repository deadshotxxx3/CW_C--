#include "BmpImage.hpp"
#include <cmath>
#include <fstream>
#include <iostream>

static constexpr uint16_t BMP_SIGNATURE = 0x4D42;
static constexpr uint8_t SUPPORTED_BITS_PER_PIXEL = 24;
static constexpr uint32_t UNCOMPRESSED_COMPRESSION = 0;
static constexpr uint8_t BYTES_PER_PIXEL = 3;

static error_marker_t checkValidBmp(BitmapFileHeader &m_file_header, BitmapInfoHeader &m_info_header)
{
    if (m_file_header.signature != BMP_SIGNATURE) {
        std::cerr << "Error: Invalid BMP signature. File is not a valid BMP image." << std::endl;
        return error_marker_t::ERR_NOTBMP;
    }
    if (m_info_header.bitsPerPixel != SUPPORTED_BITS_PER_PIXEL) {
        std::cerr << "Error: Unsupported color depth. Only 24-bit BMP files "
                     "are supported (found "
                  << m_info_header.bitsPerPixel << "-bit)." << std::endl;
        return error_marker_t::ERR_NOTBMP;
    }
    if (m_info_header.compression != UNCOMPRESSED_COMPRESSION) {
        std::cerr << "Error: Compressed BMP files are not supported. Expected "
                     "uncompressed (compression = 0)."
                  << std::endl;
        return error_marker_t::ERR_NOTBMP;
    }
    return error_marker_t::ERR_OK;
}

error_marker_t BmpImage::readBmp(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Cannot open file '" << filename << "' for reading." << std::endl;
        return error_marker_t::ERR_READING;
    }

    file.read(reinterpret_cast<char *>(&m_file_header), sizeof(m_file_header));
    file.read(reinterpret_cast<char *>(&m_info_header), sizeof(m_info_header));

    if (!file) {
        std::cerr << "Error: Failed to read BMP headers. File might be "
                     "corrupted or too short."
                  << std::endl;
        return error_marker_t::ERR_READING;
    }

    error_marker_t result = checkValidBmp(m_file_header, m_info_header);
    if (result != error_marker_t::ERR_OK) {
        return result;
    }

    uint32_t height_image = std::abs(m_info_header.height);
    uint32_t width_image = m_info_header.width;

    m_arr_pixel.resize(height_image, std::vector<Pixel>(width_image));

    file.seekg(m_file_header.pixelArrOffset, std::ios::beg);
    if (!file) {
        std::cerr << "Error: Failed to seek to pixel data offset." << std::endl;
        return error_marker_t::ERR_READING;
    }

    int row_stride = getRowStride();

    for (uint32_t i = 0; i < height_image; ++i) {
        file.read(reinterpret_cast<char *>(m_arr_pixel[i].data()), width_image * BYTES_PER_PIXEL);
        if (!file) {
            std::cerr << "Error: Unexpected end of file while reading pixel "
                         "data at row "
                      << i << "." << std::endl;
            return error_marker_t::ERR_READING;
        }

        file.ignore(row_stride - width_image * BYTES_PER_PIXEL);
    }

    file.close();
    return error_marker_t::ERR_OK;
}