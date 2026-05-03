#include "CommandLineInterface.hpp"
#include "processing_flag.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <cctype>

const int MAX_VALUE_COMPONENTS = 255;
const char DELIMETER = '.';

struct option options[] = {
    {OptionNames::INPUT, required_argument, 0, 'i'},
    {OptionNames::OUTPUT, required_argument, 0, 'o'},

    {OptionNames::COLOR_REPLACE, no_argument, 0, 0},
    {OptionNames::OLD_COLOR, required_argument, 0, 0},
    {OptionNames::NEW_COLOR, required_argument, 0, 0},

    {OptionNames::COPY, no_argument, 0, 0},
    {OptionNames::LEFT_UP, required_argument, 0, 0},
    {OptionNames::RIGHT_DOWN, required_argument, 0, 0},
    {OptionNames::DEST_LEFT_UP, required_argument, 0, 0},

    {OptionNames::SPLIT, no_argument, 0, 0},
    {OptionNames::NUMBER_X, required_argument, 0, 0},
    {OptionNames::NUMBER_Y, required_argument, 0, 0},
    {OptionNames::THICKNESS, required_argument, 0, 0},
    {OptionNames::COLOR, required_argument, 0, 0},

    {OptionNames::INFO, no_argument, 0, 0},
    {OptionNames::HELP, no_argument, 0, 'h'},
    {0, 0, 0, 0}
};

bool check_range_component(int values)
{
    if (values < 0 || values > MAX_VALUE_COMPONENTS) return false;
    return true;
}

bool check_valid_value (std::string values)
{
    for (char i: values){
        if (!isdigit(i) && i != '-') return false;
    }
    return true;
}

Coordinate getCoordinate(std::string str)
{
    std::stringstream sub_string(str);
    std::string token;
    std::vector<int> values;

    while(std::getline(sub_string,token,DELIMETER)){
        if (!check_valid_value(token)){
            std::cerr << "YOU IDIOT" << std::endl;
            exit(42);
        }
        values.push_back(std::stoi(token));
    }

    if (values.size() != 2){
        throw std::invalid_argument("Invalid coordinate format");
    }

    return Coordinate(values[0],values[1]);
}

Pixel getPixel(std::string str){
    std::stringstream sub_string(str);
    std::string token;
    std::vector<int> values;

    while(std::getline(sub_string,token,DELIMETER)){
        if (!check_valid_value(token)){
            std::cerr << "YOU IDIOT" << std::endl;
            exit(42);
        }
        values.push_back(std::stoi(token));
    }

    if (values.size() != 3){
        throw std::invalid_argument("Invalid color format");
    }

    int r = values[0],g = values[1],b = values[2];

    if (!check_range_component(r) || !check_range_component(g) || !check_range_component(b)){
        throw std::invalid_argument("Component out of range");
    }
    return Pixel(r,g,b);
}

int setInputFileIfNeeded(struct argument& arguments, int argc, char* argv[]) 
{
    if (arguments.inputName.empty() && optind < argc) {
        arguments.inputName = argv[optind];
        optind++;
    }

    if (arguments.inputName.empty()) {
        std::cerr << "Error: No input file specified\n";
        return 1;
    }
    return 0;
}

int CLI(int argc, char* argv[],struct argument& arguments)
{
    int options_index = 0;
    int c;

    arguments.flag = flags::NO_FLAG;

    while((c = getopt_long(argc,argv,"i:o:h",options,&options_index)) != -1) {
        switch(c){
            case 0:
            {
                std::string opt_name = options[options_index].name;

                process_all_flags(opt_name,optarg,arguments);

                break;

            }
            case 'o':
                if (!optarg){
                    std::cerr << "Error: output no argument" << std::endl;
                    exit(41);
                }

                arguments.outputName = optarg;
                break;
            case 'i':
                if (!optarg){
                    std::cerr << "Error: input no argument" << std::endl;
                }
                arguments.inputName = optarg;
                break;
            case 'h':
                arguments.flag = flags::FLAG_HELP;
                return 0;

            default:
                std::cerr << "Error: Unknown command" << std::endl;
                return 1;
        }

    }
    setInputFileIfNeeded(arguments,argc,argv);
    return 0;
}