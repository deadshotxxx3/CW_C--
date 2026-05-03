#include "OperationExecutor.hpp"
#include "print_help_and_info.hpp"
void operationExecutor(BmpImage& bmp,struct argument arguments){
    if (arguments.flag == flags::FLAG_MIRROR){
        bmp.mirror_image(arguments.axis,arguments.left_up,arguments.right_down);
    }
    else if (arguments.flag == flags::FLAG_COPY){
        bmp.copy_image(arguments.left_up,arguments.right_down,arguments.dest_left_up);
    }
    else if (arguments.flag == flags::FLAG_COLOR_REPLACE){
        bmp.color_replace(arguments.old_color,arguments.new_color);
    }
    else if (arguments.flag == flags::FLAG_SPLIT){
        bmp.split_image(arguments.number_x,arguments.number_y,arguments.thickness,arguments.color_split);
    }
    else if (arguments.flag == flags::FLAG_HELP){
        print_help();
    }
    else if (arguments.flag == flags::FLAG_INFO){
        bmp.print_info();
    }
}