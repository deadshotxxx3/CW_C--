#pragma once
#include <cstdint>

#pragma pack(push, 1)

/**
 * @brief BMP file header (14 bytes).
 * @details Identifies the file as a BMP and provides basic file metadata.
 *          All multi-byte fields are stored in little-endian format.
 */
struct BitmapFileHeader {
    uint16_t signature;      /**< File signature: 'BM' (0x4D42) */
    uint32_t filesize;       /**< Size of the entire file in bytes */
    uint16_t reserved1;      /**< Reserved; must be 0 */
    uint16_t reserved2;      /**< Reserved; must be 0 */
    uint32_t pixelArrOffset; /**< Byte offset from file start to pixel array */
};

/**
 * @brief BMP information header (40 bytes for v3).
 * @details Contains image dimensions, color depth, compression method, and resolution.
 *          Corresponds to the Windows BITMAPINFOHEADER structure.
 */
struct BitmapInfoHeader {
    uint32_t headerSize;          /**< Size of this header in bytes (typically 40) */
    int32_t width;                /**< Image width in pixels */
    int32_t height;               /**< Image height. Positive = bottom-up, Negative = top-down */
    uint16_t planes;              /**< Number of color planes (must be 1) */
    uint16_t bitsPerPixel;        /**< Bits per pixel (1, 4, 8, 16, 24, or 32) */
    uint32_t compression;         /**< Compression type (0 = BI_RGB/uncompressed) */
    uint32_t imageSize;           /**< Size of raw pixel data in bytes (0 for uncompressed) */
    int32_t xPixelsPerMeter;      /**< Horizontal resolution (pixels per meter) */
    int32_t yPixelsPerMeter;      /**< Vertical resolution (pixels per meter) */
    uint32_t numColors;           /**< Number of colors in palette (0 = 2^bitsPerPixel) */
    uint32_t importantColorCount; /**< Number of important colors (0 = all) */
};

/**
 * @brief Represents a 24-bit RGB/BGR pixel.
 * @details Memory layout matches BMP format: Blue, Green, Red (BGR).
 *          All components are in the range [0, 255].
 */
struct Pixel {
    uint8_t b; /**< Blue component */
    uint8_t g; /**< Green component */
    uint8_t r; /**< Red component */

    /** @brief Constructs a black pixel (0, 0, 0). */
    constexpr Pixel() noexcept : b(0), g(0), r(0) {}

    /**
     * @brief Constructs a pixel with specified RGB values.
     * @param[in] red   Red component [0-255]
     * @param[in] green Green component [0-255]
     * @param[in] blue  Blue component [0-255]
     * @note Parameters are ordered as R, G, B for convenience, but stored as B, G, R.
     */
    constexpr Pixel(uint8_t red, uint8_t green, uint8_t blue) noexcept : b(blue), g(green), r(red) {}

    /** @brief Copy assignment operator (compiler-generated is sufficient). */
    Pixel &operator=(const Pixel &other) = default;

    /**
     * @brief Checks if two pixels have identical color components.
     * @param[in] other Pixel to compare with.
     * @return true if R, G, and B match; false otherwise.
     */
    [[nodiscard]] bool operator==(const Pixel &other) const noexcept
    {
        return r == other.r && g == other.g && b == other.b;
    }
};

#pragma pack(pop)

/**
 * @brief Represents a 2D integer coordinate.
 * @details Used for defining regions, lines, and points in image processing.
 *          Values may be negative during intermediate calculations but must be
 *          clamped to [0, width-1] and [0, height-1] before accessing pixels.
 */
struct Coordinate {
    int x; /**< Horizontal position (column index) */
    int y; /**< Vertical position (row index) */

    /** @brief Constructs a coordinate at (0, 0). */
    constexpr Coordinate() noexcept : x(0), y(0) {}

    /**
     * @brief Constructs a coordinate with specified X and Y values.
     * @param[in] x_val Horizontal position
     * @param[in] y_val Vertical position
     */
    constexpr Coordinate(int x_val, int y_val) noexcept : x(x_val), y(y_val) {}

    /** @brief Copy assignment operator (compiler-generated). */
    Coordinate &operator=(const Coordinate &other) = default;

    /**
     * @brief Checks if two coordinates are identical.
     * @param[in] other Coordinate to compare with.
     * @return true if X and Y match; false otherwise.
     */
    [[nodiscard]] bool operator==(const Coordinate &other) const noexcept
    {
        return x == other.x && y == other.y;
    }

    /**
     * @brief Checks if two coordinates differ.
     * @param[in] other Coordinate to compare with.
     * @return true if X or Y differ; false otherwise.
     */
    [[nodiscard]] bool operator!=(const Coordinate &other) const noexcept
    {
        return !(*this == other);
    }
};

enum class error_marker_t {
    ERR_OK = 0,
    ERR_NOTBMP = 40,
    ERR_READING = 41,
    ERR_WRITING = 42,
    ERR_INCORRECTARG = 43,
    ERR_EXTRARGS = 44,
    ERR_FEWARGS = 45,
    ERR_FILENAME = 46
};