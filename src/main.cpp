// 标准库头文件
#include <string>
#include <iostream>


// 第三方库头文件
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "tclap/CmdLine.h"
#include "boost/asio.hpp"

#include "Worker.h"
#include "types.hpp"
#include "SandBoxConfig.h"

std::unique_ptr<GlobalStatus> lpGlobalStatus;

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
    SwitchArg ioSwitch("", "io", "Enable io connects instead of network connect");
    ValueArg<int> portArg("p", "port", "Specify port of remote connects", false, 49999, "number");
    ValueArg<std::string> hostArg("n", "name", "Specify host of remote host", false, "127.0.0.1", "remote host");

    parser.add(portArg);
    parser.add(hostArg);
    parser.add(ioSwitch);
    parser.add(verboseSwitch);

    parser.parse(argc, argv);

    if(verboseSwitch) {
        spdlog::set_level(spdlog::level::trace);
    } else {
        spdlog::set_level(spdlog::level::info);
    }

    if(ioSwitch) {
        lpGlobalStatus = std::make_unique<GlobalStatus>(std::cin, std::cout);
    }
    else {
        using boost::asio::ip::tcp;
        using boost::asio::ip::address;

        address addr = address::from_string(hostArg);
        tcp::endpoint endpoint = tcp::endpoint(addr, portArg);

        boost::asio::ip::tcp::iostream io(endpoint);

        lpGlobalStatus = std::make_unique<GlobalStatus>(io, io);
    }
}

int main (int argc, char *argv[], char *envp[]) {
    loggerInit();
    registerCmdLine(argc, argv);

    // 进入判题主循环
    work();

    return 0;
}
