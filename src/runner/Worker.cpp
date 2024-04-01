//
// Created by ubuntu on 24-4-1.
//

#include "Worker.h"

using json = nlohmann::json;

void do_single() {
    ProcessInfo info;
    json retStatus = json{};
    json jsonObject = json::parse(lpGlobalStatus->input);

    info._time_limit = jsonObject.at("time_limit").get<time_t>();
    info._memory_limit = jsonObject.at("memory_limit").get<size_t>();

    info._input_path = jsonObject.at("input_path").get<std::vector<std::string>>();
    info._output_path = jsonObject.at("output_path").get<std::vector<std::string>>();

    info._process_path = jsonObject.at("process_path").get<std::string>();
    info._args = jsonObject.at("args").get<std::vector<std::string>>();

    // TODO 完善逻辑
    // TODO 添加系统调用限制
    SandBoxStatus status = runProblem(info, SysCallRestricts());

    // TODO 完善逻辑
    if(status != SandBoxStatus::NoError) {
        retStatus["codeStatus"] = CodeStatus::RuntimeError;
    }
    else {
        retStatus["codeStatus"] = CodeStatus::Accepted;
    }
    retStatus["extra"] = "";

    lpGlobalStatus->output << retStatus.dump(4)<< std::endl;
    return;
}

void work() {
    while (true) {
        do_single();
    }
}
