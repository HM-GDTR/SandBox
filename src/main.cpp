// 标准库头文件
#include <regex>
#include <string>
#include <iostream>

// 第三方库头文件
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "tclap/CmdLine.h"

#include "server.h"
#include "SandBoxConfig.h"

int server_port;
std::string host_addr;

void loggerInit() {
    auto stderr_logger = spdlog::stderr_color_mt("sandbox");
    auto file_logger = spdlog::basic_logger_mt("basic_logger", "latest.log");

    spdlog::set_default_logger(stderr_logger);
    spdlog::get("sandbox")->sinks().push_back(file_logger->sinks().front());
}

void registerCmdLine(int argc, char *argv[]) {
    using namespace TCLAP;

    CmdLine parser("A code SandBox for Online Judge", ' ', SandBox_VERSION);

    SwitchArg verboseSwitch("", "verbose", "Enable showing the complex detail");
    ValueArg<int> portArg("p", "port", "Specify port of remote connects", false, 49999, "number");
    ValueArg<std::string> hostArg("n", "name", "Specify host of remote host", false, "0.0.0.0", "remote host");

    parser.add(portArg);
    parser.add(hostArg);
    parser.add(verboseSwitch);

    parser.parse(argc, argv);

    host_addr = hostArg.getValue();
    server_port = portArg.getValue();

    if(verboseSwitch) {
        spdlog::set_level(spdlog::level::trace);
    } else {
        spdlog::set_level(spdlog::level::info);
    }
}

int main (int argc, char *argv[], char *envp[]) {
    loggerInit();

    SandBox::main_server.run();

    return 0;
}
