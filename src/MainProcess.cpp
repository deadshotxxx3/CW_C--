#include "BmpImage.hpp"
#include "CommandLineInterface.hpp"
#include "OperationExecutor.hpp"
#include <iostream>


int main(int argc, char* argv[]){
    BmpImage bmp;
    struct argument args{};

    CLI(argc,argv,args);

    BMPerror error = bmp.readBmp(args.inputName);

    if (error != BMPerror::ERR_OK){
        return static_cast<int>(error);
    }

    operationExecutor(bmp,args);

    error = bmp.saveBmp(args.outputName);

    if (error != BMPerror::ERR_OK){
        return static_cast<int>(error);
    }

    return 0;
}
