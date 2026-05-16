#pragma once

#include "BmpImage.hpp"
#include "Error.hpp"
#include "utility"
#include <getopt.h>

enum class flags {
    NO_FLAG,
    FLAG_HELP,
    FLAG_INFO,
    FLAG_MIRROR,
    FLAG_COPY,
    FLAG_COLOR_REPLACE,
    FLAG_SPLIT,
};

namespace OptionNames
{
constexpr const char *INPUT = "input";
constexpr const char *OUTPUT = "output";
constexpr const char *COLOR_REPLACE = "color_replace";
constexpr const char *OLD_COLOR = "old_color";
constexpr const char *NEW_COLOR = "new_color";
constexpr const char *COPY = "copy";
constexpr const char *LEFT_UP = "left_up";
constexpr const char *RIGHT_DOWN = "right_down";
constexpr const char *DEST_LEFT_UP = "dest_left_up";
constexpr const char *SPLIT = "split";
constexpr const char *NUMBER_X = "number_x";
constexpr const char *NUMBER_Y = "number_y";
constexpr const char *THICKNESS = "thickness";
constexpr const char *COLOR = "color";
constexpr const char *MIRROR = "mirror";
constexpr const char *AXIS = "axis";
constexpr const char *INFO = "info";
constexpr const char *HELP = "help";
} // namespace OptionNames

struct argument {
    error_t error;
    flags flag;
    std::string inputName;
    std::string outputName = "out.bmp";
    Pixel old_color;
    Pixel new_color;
    Coordinate left_up;
    Coordinate right_down;
    Coordinate dest_left_up;
    std::string axis;
    int number_x;
    int number_y;
    int thickness;
    Pixel color_split;

    bool has_old_color = false;
    bool has_new_color = false;
    bool has_left_up = false;
    bool has_right_down = false;
    bool has_dest_left_up = false;
    bool has_axis = false;
    bool has_number_x = false;
    bool has_number_y = false;
    bool has_thickness = false;
    bool has_color_split = false;
};

std::pair<Coordinate, error_marker_t> getCoordinate(const std::string &str);
std::pair<Pixel, error_marker_t> getPixel(const std::string &str);

bool check_range_component(int values);
bool check_valid_value(std::string values);

error_marker_t CLI(int argc, char *argv[], struct argument &arguments);