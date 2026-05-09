#include "CommandLineInterface.hpp"
#include "Error.hpp"
#include "processing_flag.hpp"
#include <cctype>
#include <iostream>
#include <sstream>
#include <vector>

const int MAX_VALUE_COMPONENTS = 255;
const char DELIMETER = '.';

struct option options[] = {{OptionNames::INPUT, required_argument, 0, 'i'},
                           {OptionNames::OUTPUT, required_argument, 0, 'o'},

                           {OptionNames::COLOR_REPLACE, no_argument, 0, 0},
                           {OptionNames::OLD_COLOR, required_argument, 0, 0},
                           {OptionNames::NEW_COLOR, required_argument, 0, 0},

                           {OptionNames::COPY, no_argument, 0, 0},
                           {OptionNames::LEFT_UP, required_argument, 0, 0},
                           {OptionNames::RIGHT_DOWN, required_argument, 0, 0},
                           {OptionNames::DEST_LEFT_UP, required_argument, 0, 0},

                           {OptionNames::MIRROR, no_argument, 0, 0},
                           {OptionNames::AXIS, required_argument, 0, 0},

                           {OptionNames::SPLIT, no_argument, 0, 0},
                           {OptionNames::NUMBER_X, required_argument, 0, 0},
                           {OptionNames::NUMBER_Y, required_argument, 0, 0},
                           {OptionNames::THICKNESS, required_argument, 0, 0},
                           {OptionNames::COLOR, required_argument, 0, 0},

                           {OptionNames::INFO, no_argument, 0, 0},
                           {OptionNames::HELP, no_argument, 0, 'h'},
                           {0, 0, 0, 0}};

bool check_range_component(int values)
{
    if (values < 0 || values > MAX_VALUE_COMPONENTS)
        return false;
    return true;
}

bool check_valid_value(std::string values)
{
    for (char i : values) {
        if (!isdigit(i) && i != '-')
            return false;
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

    if (values.size() != 2) {
        std::cerr << "Invalid coordinate format: expected x.y\n";
        return {Coordinate(), error_marker_t::ERR_INCORRECTARG};
    }

    return {Coordinate(values[0], values[1]), error_marker_t::ERR_OK};
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

    if (values.size() != 3) {
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

error_marker_t setInputFileIfNeeded(struct argument &arguments, int argc, char *argv[])
{
    if (arguments.inputName.empty() && optind < argc) {
        arguments.inputName = argv[optind];
        optind++;
    }

    if (arguments.inputName.empty()) {
        std::cerr << "Error: No input file specified\n";
        return error_marker_t::ERR_INCORRECTARG;
    }
    return error_marker_t::ERR_OK;
}

error_marker_t CLI(int argc, char *argv[], struct argument &arguments)
{
    int options_index = 0;
    int c;

    arguments.flag = flags::NO_FLAG;

    while ((c = getopt_long(argc, argv, "i:o:h", options, &options_index)) != -1) {
        switch (c) {
            case 0: {
                std::string opt_name = options[options_index].name;

                error_marker_t error = process_all_flags(opt_name, optarg, arguments);
                if (error != error_marker_t::ERR_OK) {
                    return error;
                }

                break;
            }
            case 'o':
                if (!optarg) {
                    std::cerr << "Error: output no argument" << std::endl;
                    return error_marker_t::ERR_INCORRECTARG;
                }

                arguments.outputName = optarg;
                break;
            case 'i':
                if (!optarg) {
                    std::cerr << "Error: input no argument" << std::endl;
                    return error_marker_t::ERR_INCORRECTARG;
                }
                arguments.inputName = optarg;
                break;
            case 'h':
                arguments.flag = flags::FLAG_HELP;
                return error_marker_t::ERR_OK;

            default:
                std::cerr << "Error: Unknown command" << std::endl;
                return error_marker_t::ERR_INCORRECTARG;
        }
    }

    error_marker_t result = setInputFileIfNeeded(arguments, argc, argv);
    if (result != error_marker_t::ERR_OK) {
        return result;
    }

    return error_marker_t::ERR_OK;
}