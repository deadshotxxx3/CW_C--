#include "BmpImage.hpp"
#include "Error.hpp"
#include <fstream>
#include <iostream>

const int BMPSIGNATURE = 0x4D42;

error_marker_t checkValidBmp(BitmapFileHeader &m_file_header, BitmapInfoHeader &m_info_header)
{
    if (m_file_header.signature != BMPSIGNATURE) {
        std::cerr << "Error: Invalid BMP signature. File is not a valid BMP image." << std::endl;
        return error_marker_t::ERR_NOTBMP;
    }
    if (m_info_header.bitsPerPixel != 24) {
        std::cerr << "Error: Unsupported color depth. Only 24-bit BMP files "
                     "are supported (found "
                  << m_info_header.bitsPerPixel << "-bit)." << std::endl;
        return error_marker_t::ERR_NOTBMP;
    }
    if (m_info_header.compression != 0) {
        std::cerr << "Error: Compressed BMP files are not supported. Expected "
                     "uncompressed (compression = 0)."
                  << std::endl;
        return error_marker_t::ERR_NOTBMP;
    }
    return error_marker_t::ERR_OK;
}

int BmpImage::getRowStride() const
{
    return (m_info_header.bitsPerPixel * m_info_header.width + 31) / 32 * 4;
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

    if (result != error_marker_t::ERR_OK)
        return result;

    uint32_t H = abs(m_info_header.height);
    uint32_t W = m_info_header.width;

    arr_pixels.resize(H, std::vector<Pixel>(W));

    file.seekg(m_file_header.pixelArrOffset, std::ios::beg);
    if (!file) {
        std::cerr << "Error: Failed to seek to pixel data offset." << std::endl;
        return error_marker_t::ERR_READING;
    }

    int row_stride = getRowStride();

    for (uint32_t i = 0; i < H; ++i) {
        file.read(reinterpret_cast<char *>(arr_pixels[i].data()), m_info_header.width * 3);
        if (!file) {
            std::cerr << "Error: Unexpected end of file while reading pixel "
                         "data at row "
                      << i << "." << std::endl;
            return error_marker_t::ERR_READING;
        }

        file.ignore(row_stride - W * 3);
    }

    file.close();

    return error_marker_t::ERR_OK;
}