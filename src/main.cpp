// 标准库头文件
#include <string>
#include <iostream>

// 第三方库头文件
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "tclap/CmdLine.h"

#include "Server.h"
#include "types.hpp"
#include "ThreadInfo.hpp"
#include "SandBoxConfig.h"
#include "BaseException.h"

ThreadInfo threadInfo;
std::unique_ptr<GlobalStatus> lpGlobalStatus;

void loggerInit() {
    threadInfo.regThread(gettid(), "Main");
    auto stderr_logger = spdlog::stderr_color_mt("sandbox");
    auto file_logger = spdlog::basic_logger_mt("basic_logger", "latest.log");

    spdlog::set_default_logger(stderr_logger);
    spdlog::get("sandbox")->sinks().push_back(file_logger->sinks().front());
}

void registerCmdLine(int argc, char *argv[]) {
    using namespace TCLAP;

    CmdLine parser("A code SandBox for Online Judge", ' ', SandBox_VERSION);

    SwitchArg verboseSwitch("", "verbose", "Enable showing the complex detail");

    parser.add(verboseSwitch);

    parser.parse(argc, argv);

    if(verboseSwitch) {
        spdlog::set_level(spdlog::level::trace);
    } else {
        spdlog::set_level(spdlog::level::info);
    }
}

int main (int argc, char *argv[], char *envp[]) {
    loggerInit();
    registerCmdLine(argc, argv);

    // 进入服务器阶段
    SandBox::Server sandBox{};
    sandBox.run();

    return 0;
}
