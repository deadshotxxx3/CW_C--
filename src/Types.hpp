#pragma once
#include <cstdint>

#pragma pack(push, 1)

struct BitmapFileHeader {
    uint16_t signature;
    uint32_t filesize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t pixelArrOffset;
};

struct BitmapInfoHeader {
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t numColors;
    uint32_t importantColorCount;
};

struct Pixel {
    uint8_t b;
    uint8_t g;
    uint8_t r;

    Pixel() : b(0), g(0), r(0) {}

    Pixel(uint8_t r, uint8_t g, uint8_t b) : b(b), g(g), r(r) {}

    Pixel &operator=(const Pixel &other)
    {
        if (this != &other) {
            r = other.r;
            g = other.g;
            b = other.b;
        }
        return *this;
    }

    bool operator==(const Pixel &other)
    {
        if ((*this).r == other.r && (*this).g == other.g && (*this).b == other.b)
            return true;
        return false;
    }
};

#pragma pack(pop)

struct Coordinate {
    int x;
    int y;

    Coordinate() : x(0), y(0) {}

    Coordinate(int x, int y) : x(x), y(y) {}

    Coordinate &operator=(const Coordinate &other)
    {
        if (this != &other) {
            x = other.x;
            y = other.y;
        }
        return *this;
    }
};