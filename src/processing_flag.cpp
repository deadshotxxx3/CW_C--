#include "processing_flag.hpp"
#include "CommandLineInterface.hpp"
#include <iostream>

int process_coordinate_flag(const std::string& opt_name, const char* optarg, struct argument& arguments) {
    if (opt_name == OptionNames::LEFT_UP) {
        if (arguments.flag == flags::FLAG_MIRROR || arguments.flag == flags::FLAG_COPY) {
            arguments.left_up = getCoordinate(optarg);
            return 0;
        }
        std::cerr << "Error: --left_up not valid for current operation\n";
        return 2;
    }

    if (opt_name == OptionNames::RIGHT_DOWN) {
        if (arguments.flag == flags::FLAG_MIRROR || arguments.flag == flags::FLAG_COPY) {
            arguments.right_down = getCoordinate(optarg);
            return 0;
        }
        std::cerr << "Error: --right_down not valid for current operation\n";
        return 2;
    }

    return 1;
}

int processing_color_flags(const std::string& opt_name, const char* optarg, struct argument& arguments){
    if (opt_name == OptionNames::COLOR_REPLACE) arguments.flag = flags::FLAG_COLOR_REPLACE;
    else if (opt_name == OptionNames::OLD_COLOR){
        if (arguments.flag != flags::FLAG_COLOR_REPLACE){
            std::cerr << "Error: --old_color requires --color_replace first" << std::endl;
            return 2;
        }
        arguments.old_color = getPixel(optarg);
    }
    else if (opt_name == OptionNames::NEW_COLOR){
        if (arguments.flag != flags::FLAG_COLOR_REPLACE){
            std::cerr << "Error: --new_color requires --color_replace first" << std::endl;
            return 2;
        }
        arguments.new_color = getPixel(optarg);
    }
    return 0;
}                         

int processing_copy_flags(const std::string& opt_name, const char* optarg, struct argument& arguments){
    if(opt_name == OptionNames::COPY) arguments.flag = flags::FLAG_COPY;

    else if(opt_name == OptionNames::DEST_LEFT_UP){
        arguments.dest_left_up = getCoordinate(optarg);
    }

    process_coordinate_flag(opt_name,optarg,arguments);

    return 0;
}

int processing_mirror_flag(const std::string& opt_name,const char* optarg, struct argument& arguments){
    if(opt_name == OptionNames::MIRROR) arguments.flag = flags::FLAG_MIRROR;

    else if(opt_name == OptionNames::AXIS){
        if (optarg != "x" || optarg != "y"){
            std::cout << "Error";
        }
        arguments.axis = optarg;
    }

    process_coordinate_flag(opt_name,optarg,arguments);

    return 0;

}

int processing_split_flag(const std::string& opt_name,const char* optarg, struct argument& arguments){
    if(opt_name == OptionNames::SPLIT) arguments.flag = flags::FLAG_SPLIT;

    else if (opt_name == OptionNames::NUMBER_X){
        int value = std::stoi(optarg);
        if (value <= 1){
            std::cerr << "Error: number_x must be greater that 1\n";
            return 42;
        }
        arguments.number_x = value;
    }

    else if (opt_name == OptionNames::NUMBER_Y){
        int value = std::stoi(optarg);
        if (value <= 1){
            std::cerr << "Error: number_y must be greater that 1\n";
            return 42;
        }
        arguments.number_y = value;
    }

    else if (opt_name == OptionNames::THICKNESS){
        int value = std::stoi(optarg);
        if (value < 0){
            std::cerr << "Error: thickness must be greater that 0\n";
            return 42;
        }
        arguments.thickness = value;
    }

    else if (opt_name == OptionNames::COLOR){
        arguments.color_split = getPixel(optarg);
    }

    return 0;
}

int process_all_flags(const std::string& opt_name, const char* optarg, struct argument& arguments) {
    int result;

    result = processing_color_flags(opt_name, optarg, arguments);

    result = processing_copy_flags(opt_name, optarg, arguments);

    result = processing_mirror_flag(opt_name, optarg, arguments);

    result = processing_split_flag(opt_name, optarg, arguments);

    result = process_coordinate_flag(opt_name, optarg, arguments);

    if (opt_name == OptionNames::INFO){
        arguments.flag = flags::FLAG_INFO;
    }

    return 0;
}