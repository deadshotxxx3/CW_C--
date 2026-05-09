#include "BmpImage.hpp"
#include "CommandLineInterface.hpp"
#include "OperationExecutor.hpp"
#include "print_help_and_info.hpp"
#include <iostream>

int main(int argc, char* argv[]){
    BmpImage bmp;
    struct argument args{};

    CLI(argc,argv,args);

    if (args.flag == flags::FLAG_HELP){
        print_help();
        return 0;
    }
    if (args.flag == flags::FLAG_INFO) return 0;
    
    // std::cout << "left_up: " << args.left_up.x << "." << args.left_up.y << std::endl;
    // std::cout << "right_down: " << args.right_down.x << "." << args.right_down.y << std::endl;
    // std::cout << "dest_left_up: " << args.dest_left_up.x << "." << args.dest_left_up.y << std::endl;

    error_marker_t error = bmp.readBmp(args.inputName);

    // BitmapInfoHeader hui = bmp.getInfoHeader();
    // std::cout << "m_info_header.height: " << hui.height << std::endl;
    // std::cout << "m_info_header.width: " << hui.width << std::endl;

    if (error != error_marker_t::ERR_OK){
        return static_cast<int>(error);
    }

    operationExecutor(bmp,args);

    error = bmp.saveBmp(args.outputName);

    if (error != error_marker_t::ERR_OK){
        return static_cast<int>(error);
    }

    // BmpImage bmp_out; 
    // bmp_out.readBmp("./gay.bmp");

    // BitmapInfoHeader info = bmp_out.getInfoHeader();

    // for (uint32_t y = 0; y < info.height;y++){
    //     for (uint32_t x = 0; x < info.width;x++){
    //         if (bmp.arr_pixels[y][x] == bmp_out.arr_pixels[y][x]){
    //             bmp.arr_pixels[y][x] = Pixel{255,255,255};
    //         }
    //         else{
    //             bmp.arr_pixels[y][x] = Pixel{0,0,0};
    //         }
    //     }
    // }
    // bmp.saveBmp("./result.bmp");


    return 0;
}
