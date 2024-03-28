//
// Created by ubuntu on 24-3-28.
//

#include "src/include/Runner.h"

/**
 * 运行某一组测试样例
 * @param info 题目限制
 * @param example_idx 样例编号
 * @return 返回沙箱当前状态 (如果执行失败)
 */
SandBoxStatus runSample(ProcessInfo info, SysCallRestricts restricts, size_t example_idx) {
    // 装载新可执行文件需要的命令行参数
    std::vector<const char*> args;
    // 新可执行文件的路径
    const char* exec_path = info._process_path.c_str();
    // 输入输出文件的路径
    std::string in_file_path(info._input_path[example_idx]), out_file_path(info._output_path[example_idx]);

    // 将所需参数放入vector中
    args.reserve(info._args.size());
    for(const std::string& str : info._args) {
        args.emplace_back(str.c_str());
    }
    args.push_back(nullptr);

    // 将指针取出以调用execv系统调用
    char* const* arg_list = const_cast<char* const*>(args.data());

    // TODO 添加资源限制

    // 输入输出重定向(如果需要的话) TODO 异常处理
    if(not in_file_path.empty()) freopen(in_file_path.c_str(), "r", stdin);
    if(not out_file_path.empty()) freopen(out_file_path.c_str(), "w", stdout);

    // 尝试执行目标执行文件
    int err = execv(exec_path, arg_list);

    // TODO 记录失败日志

    return SandBoxStatus::ExecError;
}

/**
 * 运行某个问题的评测
 * @param info 问题信息与限制
 * @param restricts 系统调用限制
 * @return 返回沙箱当前状态
 */
SandBoxStatus runProblem(ProcessInfo info, SysCallRestricts restricts) {
    // 需要保证 输入文件个数与输出文件相同
    assert(info._input_path.size() == info._output_path.size());

    size_t samp_len = info._input_path.size();
    for(size_t samp_idx = 0; samp_idx < samp_len; samp_idx++) {
        pid_t pid = fork();
        // 如果fork失败
        if (pid < 0) return SandBoxStatus::ForkError;

        // 如果是子进程
        if (pid == 0) {
            return runSample(info, restricts, samp_idx);
        }

        // 如果是父进程
        // 监控子进程代码
        int ret_code = 0;
        int ret = waitpid(pid, &ret_code, 0);

        // 正常情况
        if(ret == pid) {

        }
        // 非正常情况
        else {

        }
    }
    return SandBoxStatus::NoError;
}
