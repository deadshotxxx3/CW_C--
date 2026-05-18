#include "ArgumentParser.hpp"

#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>

static constexpr int MAX_VALUE_COMPONENTS = 255;
static constexpr int CNT_COMPINENTS = 3;
static constexpr int CNT_COORDINATE = 2;
static constexpr char DELIMETER = '.';

bool check_range_component(int values)
{
    if (values < 0 || values > MAX_VALUE_COMPONENTS)
        return false;
    return true;
}

bool check_valid_value(const std::string &values)
{
    if (values.empty())
        return false;

    size_t start = 0;
    if (values[0] == '-') {
        if (values.length() == 1)
            return false;
        start = 1;
    }

    for (size_t i = start; i < values.length(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(values[i]))) {
            return false;
        }
    }
    return true;
}

std::pair<Coordinate, error_marker_t> getCoordinate(const std::string &str)
{
    std::stringstream sub_string(str);
    std::string token;
    std::vector<int> values;

    while (std::getline(sub_string, token, DELIMETER)) {
        if (!check_valid_value(token)) {
            std::cerr << "Invalid coordinate format: non-numeric character\n";
            return {Coordinate(), error_marker_t::ERR_INCORRECTARG};
        }
        values.push_back(std::stoi(token));
    }

    if (values.size() != CNT_COORDINATE) {
        std::cerr << "Invalid coordinate format: expected x.y\n";
        return {Coordinate(), error_marker_t::ERR_INCORRECTARG};
    }

    return {Coordinate(values[0], values[1]),
            error_marker_t::ERR_OK}; // values[0] = x coordinate, values[1] = y;
}

std::pair<Pixel, error_marker_t> getPixel(const std::string &str)
{
    std::stringstream sub_string(str);
    std::string token;
    std::vector<int> values;

    while (std::getline(sub_string, token, DELIMETER)) {
        if (!check_valid_value(token)) {
            std::cerr << "Invalid color format: non-numeric character\n";
            return {Pixel(), error_marker_t::ERR_INCORRECTARG};
        }
        values.push_back(std::stoi(token));
    }

    if (values.size() != CNT_COMPINENTS) {
        std::cerr << "Invalid color format: expected r.g.b\n";
        return {Pixel(), error_marker_t::ERR_INCORRECTARG};
    }

    int r = values[0], g = values[1], b = values[2];

    if (!check_range_component(r) || !check_range_component(g) || !check_range_component(b)) {
        std::cerr << "Color component out of range (0-255)\n";
        return {Pixel(), error_marker_t::ERR_INCORRECTARG};
    }
    return {Pixel(r, g, b), error_marker_t::ERR_OK};
}