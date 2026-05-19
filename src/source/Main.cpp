#include "BmpImage.hpp"
#include "CommandLineInterface.hpp"
#include "OperationExecutor.hpp"
#include "PrintHelpAndInfo.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    BmpImage bmp;
    argument args{};

    auto check = [](error_marker_t err) -> int {
        return (err == error_marker_t::ERR_OK) ? 0 : static_cast<int>(err);
    };

    if (int res = check(CLI(argc, argv, args)))
        return res;
    if (args.flag == flags::FLAG_HELP) {
        print_help();
        return 0;
    }

    if (int res = check(bmp.readBmp(args.inputName)))
        return res;

    operationExecutor(bmp, args);

    if (int res = check(bmp.saveBmp(args.outputName)))
        return res;

    return 0;
}