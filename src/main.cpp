// 标准库头文件
#include <string>
#include <iostream>

#include <boost/asio.hpp>

// 第三方库头文件
#include "json.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "tclap/CmdLine.h"

#include "types.hpp"
#include "SandBoxConfig.h"

using json = nlohmann::json;
using boost::asio::ip::tcp;

void loggerInit() {
    auto stderr_logger = spdlog::stderr_color_mt("sandbox");
    auto file_logger = spdlog::basic_logger_mt("basic_logger", "latest.log");

    spdlog::set_default_logger(stderr_logger);
    spdlog::get("stderr")->sinks().push_back(file_logger->sinks().front());
}

void registerCmdLine(int argc, char *argv[]) {
    using namespace TCLAP;

    CmdLine parser("A code SandBox for Online Judge", ' ', SandBox_VERSION);
    ValueArg<int> portName("p", "port", "Specify port of remote connects", false, 49999, "number");
    ValueArg<std::string> hostName("H", "host", "Specify host of remote connects", false, "127.0.0.1", "dns name or ip address");
    SwitchArg verbose("v", "verbose", "Enable showing the complex detail");

    parser.add(portName);
    parser.add(hostName);
    parser.add(verbose);

    parser.parse(argc, argv);

    if(verbose) {
        spdlog::set_level(spdlog::level::trace);
    } else {
        spdlog::set_level(spdlog::level::info);
    }
}

int main (int argc, char *argv[], char *envp[]) {
    loggerInit();
    registerCmdLine(argc, argv);

    return 0;
}
