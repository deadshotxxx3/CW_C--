#pragma once

/**
 * @file BmpImage.hpp
 * @brief Core class for managing, processing, and persisting 24-bit BMP images.
 * @details Provides a complete abstraction over the BMP file format, handling
 *          binary headers, pixel data storage, and common image manipulation
 *          operations such as mirroring, copying, color replacement, and grid splitting.
 */

#include "Types.hpp"
#include <string>
#include <vector>

/**
 * @class BmpImage
 * @brief In-memory representation of a BMP image with manipulation capabilities.
 * @details Stores file and info headers alongside a 2D pixel matrix.
 *          The matrix follows the standard BMP bottom-up orientation, where
 *          the first row index corresponds to the bottom of the image.
 */
class BmpImage {
private:
    BitmapFileHeader m_file_header; /**< <tt>BITMAPFILEHEADER</tt>: Contains file signature, size, and pixel
                                       data offset. */
    BitmapInfoHeader m_info_header; /**< <tt>BITMAPINFOHEADER</tt>: Contains image dimensions, color depth,
                                       and compression type. */
    std::vector<std::vector<Pixel>>
        arr_pixels; /**< 2D pixel buffer storing image data. Indexed as [row][column]. */

    /**
     * @brief Updates BMP headers and recalculates file size after dimension changes.
     * @details Modifies internal metadata to reflect new image dimensions. Computes the
     *          padded row stride using integer arithmetic: ((width * 3 + 3) / 4) * 4.
     *          This formula rounds the raw byte count up to the nearest 4-byte boundary,
     *          as mandated by the BMP specification.
     *          - * 3: Converts pixel width to bytes (24-bit BMP = 3 bytes per pixel).
     *          - + 3: Compensates for truncating integer division, ensuring rounding up.
     *          - / 4 * 4: Normalizes the result to the exact 4-byte alignment step.
     * @param[in] new_width  New image width in pixels.
     * @param[in] new_height New image height in pixels.
     */
    void updateDimensions(int new_width, int new_height)
    {
        int row_stride = ((new_width * 3 + 3) / 4) * 4;
        m_info_header.width = new_width;
        m_info_header.height = new_height;
        m_info_header.imageSize = row_stride * new_height;
        m_file_header.filesize =
            sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + m_info_header.imageSize;
    }

public:
    /** @brief Default constructor. Initializes an empty image object. */
    BmpImage() = default;

    /**
     * @brief Calculates the byte stride for a single image row, including BMP padding.
     * @details BMP format requires each row to be aligned to a 4-byte boundary.
     *          This method uses the bitwise rounding formula: (width * 3 + 3) & ~3.
     *          - Adding 3 ensures that any non-zero remainder pushes the value
     *            into the next 4-byte block before truncation.
     *          - Bitwise AND with ~3 (binary ...11111100) clears the lowest two bits,
     *            effectively rounding the result down to the nearest multiple of 4.
     * @return Row stride in bytes (always a multiple of 4).
     */
    int getRowStride() const
    {
        int raw_bytes = m_info_header.width * 3;
        return (raw_bytes + 3) & ~3;
    }

    /**
     * @brief Retrieves the BMP information header.
     * @details Provides read-only access to image metadata such as width, height,
     *          bits per pixel, and compression method.
     * @return Constant reference to the <tt>BitmapInfoHeader</tt> structure.
     */
    const BitmapInfoHeader &getInfoHeader() const
    {
        return m_info_header;
    }

    /**
     * @brief Retrieves the BMP file header.
     * @details Provides read-only access to file-level metadata, including the
     *          file size and the offset to the pixel array.
     * @return Constant reference to the <tt>BitmapFileHeader</tt> structure.
     */
    const BitmapFileHeader &getFileHeader() const
    {
        return m_file_header;
    }

    /**
     * @brief Retrieves the internal 2D pixel matrix.
     * @details Returns a constant reference to the pixel buffer for read-only
     *          access. The matrix is organized in bottom-up order matching BMP standards.
     * @return Constant reference to the <tt>std::vector<std::vector<Pixel>></tt> buffer.
     */
    const std::vector<std::vector<Pixel>> &getArrPixels() const
    {
        return arr_pixels;
    }

    /**
     * @brief Loads and parses a BMP image from the specified file path.
     * @details Reads binary headers and pixel data into memory. Validates the
     *          file signature, checks for 24-bit uncompressed format, and handles
     *          row padding automatically.
     * @param[in] filename Absolute or relative path to the source .bmp file.
     * @return <tt>ERR_OK</tt> on successful load, or a specific error code on failure.
     */
    error_marker_t readBmp(const std::string &filename);

    /**
     * @brief Serializes the current image state to a BMP file.
     * @details Writes file/info headers and pixel data to disk. Applies necessary
     *          4-byte row padding and updates header fields (e.g., file size) before writing.
     * @param[in] filename Absolute or relative path for the output .bmp file.
     * @return <tt>ERR_OK</tt> on successful save, or a specific error code on failure.
     */
    error_marker_t saveBmp(const std::string &filename);

    /**
     * @brief Mirrors a rectangular region of the image along a specified axis.
     * @details Performs an in-place reflection of pixels within the bounds defined
     *          by <tt>left_up</tt> and <tt>right_down</tt>. Supports horizontal ('x')
     *          and vertical ('y') axis mirroring.
     * @param[in] axis Axis of reflection. Accepts "x" for horizontal or "y" for vertical flip.
     * @param[in] left_up Top-left corner coordinate defining the region boundary.
     * @param[in] right_down Bottom-right corner coordinate defining the region boundary.
     */
    void mirror_image(std::string &axis, Coordinate &left_up, Coordinate &right_down);

    /**
     * @brief Copies a rectangular region from a source position to a destination.
     * @details Extracts pixels from the source rectangle and pastes them at the
     *          destination offset. Handles overlapping source/destination regions safely.
     * @param[in] left_up Top-left coordinate of the source region.
     * @param[in] right_down Bottom-right coordinate of the source region.
     * @param[in] dest_left_up Top-left coordinate where the copied region will be placed.
     */
    void copy_image(Coordinate &left_up, Coordinate &right_down, Coordinate &dest_left_up);

    /**
     * @brief Replaces all pixels matching a specific color with a new color.
     * @details Iterates through the entire pixel buffer and performs an exact match
     *          comparison. Updates matching pixels in-place.
     * @param[in] old_color The target RGB color to search for.
     * @param[in] new_color The replacement RGB color to apply.
     */
    void color_replace(const Pixel &old_color, const Pixel &new_color);

    /**
     * @brief Splits the image into a grid by expanding the canvas and adding separator lines.
     * @details Increases the overall image dimensions to accommodate horizontal and vertical
     *          grid lines of the specified thickness. The original image content remains intact
     *          within the resulting cells, while lines are rendered in the newly allocated space
     *          between sections.
     * @param[in] cntLineX Number of vertical divisions (columns) to create.
     * @param[in] cntLineY Number of horizontal divisions (rows) to create.
     * @param[in] thickness Width of the grid lines in pixels.
     * @param[in] colorLine RGB color used for drawing the grid lines.
     */
    void split_image(int cntLineX, int cntLineY, int thickness, const Pixel &colorLine);

    /**
     * @brief Outputs image metadata to the standard console.
     * @details Prints formatted information including file size, dimensions,
     *          color depth, compression type, and pixel array offset.
     */
    void print_info() const;
};