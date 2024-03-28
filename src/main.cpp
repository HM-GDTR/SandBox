#include <string>
#include <iostream>

#include "types.hpp"
#include "SandBoxConfig.h"
#include "tclap/CmdLine.h"

int main (int argc, char *argv[], char *envp[]) {
    TCLAP::CmdLine parser("A code SandBox for Online Judge", ' ', SandBox_VERSION);
    TCLAP::ValueArg<std::string> pipeName("p", "pipe", "Name of the pipeline used to transmit information", false, "", "string");
    parser.add(pipeName);
    parser.parse(argc, argv);



    return 0;
}
