#include "BmpImage.hpp"
#include "CommandLineInterface.hpp"
#include "OperationExecutor.hpp"
#include "print_help_and_info.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    BmpImage bmp;

    struct argument args {};

    auto error = CLI(argc, argv, args);

    if (error != error_marker_t::ERR_OK) {
        return static_cast<int>(error);
    }

    if (args.flag == flags::FLAG_HELP) {
        print_help();
        return 0;
    }
    if (args.flag == flags::FLAG_INFO)
        return 0;
    error = bmp.readBmp(args.inputName);

    if (error != error_marker_t::ERR_OK) {
        return static_cast<int>(error);
    }
    operationExecutor(bmp, args);


    error = bmp.saveBmp(args.outputName);

    if (error != error_marker_t::ERR_OK) {
        return static_cast<int>(error);
    }

    return 0;
}
