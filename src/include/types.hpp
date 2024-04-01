//
// Created by ubuntu on 24-3-28.
//

#ifndef SANDBOX_TYPES_HPP
#define SANDBOX_TYPES_HPP

#include <string>
#include <vector>
#include <memory>

#include "status.hpp"

struct GlobalStatus {
    std::istream& input;
    std::ostream& output;

    GlobalStatus(std::istream &input, std::ostream &output) : input(input), output(output) {}
};
extern std::unique_ptr<GlobalStatus> lpGlobalStatus;

/**
 * 对于系统调用的限制
 */
struct SysCallRestricts {
};

/**
 * 代码运行信息
 */
struct CodeResult {
    // 程序返回状态
    CodeStatus codeStatus;

    // 程序额外信息
    std::string extra;
};

/**
 * 程序的相关信息
 */
struct ProcessInfo {
    // 运行程序的路径
    std::string _process_path;
    // 运行程序所需要的命令行参数
    std::vector<std::string> _args;

    // 输入输出文件路径
    std::vector<std::string> _input_path, _output_path;

    // 时间限制
    time_t _time_limit;
    // 空间限制
    size_t _memory_limit;

};

#endif //SANDBOX_TYPES_HPP
