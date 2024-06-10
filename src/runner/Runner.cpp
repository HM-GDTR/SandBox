//
// Created by rainbowx on 24-6-9.
//

#include "Runner.h"

#include <ctime>
#include <chrono>
#include <thread>
#include <vector>
#include <string>
#include <exception>

#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/resource.h>

#include "spdlog/spdlog.h"
#include "boost/test/debug.hpp"

#include "server.h"
#include "Errors.hpp"
#include "restrict.h"
#include "SandBox.hpp"

namespace SandBox {

    Runner::Runner(size_t max_task): max_task_(max_task), n_task_() { }

    nlohmann::json Runner::run_single_task(const nlohmann::json& task_describe) {
        time_t max_real_time_val = -1;
        // exe_path arg was necessary
        if(not task_describe.contains("exe_path") or not task_describe["exe_path"].is_string()) {
            nlohmann::json result{};

            result["signal"] = 0;
            result["error"] = OJStatus::INVALID_CONFIG;
            result["result"] = RunStatus::InternalError;
            result["exit_code"] = 0;

            result["resource_usage"] = nlohmann::json{};

            return result;
        }
        if(task_describe.contains("max_real_time")) {
            const auto& max_real_time = task_describe.at("max_real_time");
            if(not max_real_time.is_number_unsigned()) {
                ERROR_EXIT(OJStatus::INVALID_CONFIG);
            }
            max_real_time_val = max_real_time;
        }

        pid_t pid = fork();
        // fail when fork
        if(pid < 0) {
            spdlog::error("Fail to call fork, reason: {}.", strerror(errno));

            nlohmann::json result{};
            result["signal"] = 0;
            result["error"] = OJStatus::FORK_FAILED;
            result["result"] = RunStatus::NoError;
            result["exit_code"] = 0;

            return result;
        }
        spdlog::trace("Fork successfully.");

        // ------------------------------------child process-----------------------------------
        if(pid == 0) {
            // setup child process
            do_restricts(task_describe);

            // run specified task
            run_child(task_describe);
            raise(SIGUSR1);

            // error handling
            exit(static_cast<int>(OJStatus::EXECVE_FAILED));
        }
        // ------------------------------------child process-----------------------------------

        // -----------------------------------parent Process-----------------------------------
        int status = 0;
        bool is_timeout = false;
        rusage resource_usage{};
        time_t start_time{}, end_time{};
        nlohmann::json result{}, resource{};

        // return status
        int signal = 0, exit_code = 0;
        OJStatus oj_error = OJStatus::SUCCESS;
        RunStatus run_result = RunStatus::NoError;

        start_time = time(nullptr);

        // set timer to kill timeout task.
        if(max_real_time_val!= -1) {
            std::thread{[&](int milliseconds) {
                std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
                is_timeout = kill(pid, SIGKILL) == 0;
            }, std::min(max_real_time_val*2, max_real_time_val+1000)}.detach();
        }

        status = 0;
        wait4(pid, &status, WSTOPPED, &resource_usage);

        end_time = time(nullptr);

        resource["real_time_cost"] = end_time - start_time;
        resource["memory_cost"] = resource_usage.ru_maxrss * 1024;
        resource["cpu_time_cost"] = static_cast<time_t>(resource_usage.ru_utime.tv_sec * 1000 + resource_usage.ru_utime.tv_usec / 1000);

        // if not stopped, kill it
        if(not (WIFEXITED(status) or WIFSIGNALED(status))) {
            kill(pid, SIGKILL);
        }

        // if killed by timer
        if(is_timeout) {
            signal = 0;
            oj_error = OJStatus::SUCCESS;
            run_result = RunStatus::TimeLimitExceedError;
            exit_code = WEXITSTATUS(status);
            goto report;
        }
        // if stopped by signal SIGSYS
        if ((WIFSTOPPED(status) and WSTOPSIG(status) == SIGSYS) or (WIFSIGNALED(status) and WTERMSIG(status) == SIGSYS)) {
            signal = WTERMSIG(status);
            if(WIFSTOPPED(status)) {
                siginfo_t siginfo;
                signal = WSTOPSIG(status);
                ptrace(PTRACE_GETSIGINFO, pid, nullptr, &siginfo);
                spdlog::info("Child process received SIGSYS, syscall: {}, address: {}", siginfo.si_syscall, siginfo.si_call_addr);
            }

            oj_error = OJStatus::SUCCESS;
            run_result = RunStatus::RestrictedError;
            exit_code = WEXITSTATUS(status);
            goto report;
        }
        // if exited normally
        if(WIFEXITED(status) and WEXITSTATUS(status) == 0) {
            signal = 0;
            oj_error = OJStatus::SUCCESS;
            run_result = RunStatus::NoError;
            exit_code = WEXITSTATUS(status);
            goto report;
        }

        signal = 0;
        oj_error = OJStatus::SUCCESS;
        run_result = RunStatus::RuntimeError;
        exit_code = WEXITSTATUS(status);
        goto report;


    report:
        // ensure child process is die
        ensure(kill(pid, 0) != 0);

        result["signal"] = signal;
        result["error"] = oj_error;
        result["result"] = run_result;
        result["exit_code"] = exit_code;

        result["resource_usage"] = resource;

        return result;
        // -----------------------------------parent Process-----------------------------------
    }

    task_t Runner::add_task(nlohmann::json task_describe) {
        {
            std::scoped_lock<std::mutex> scoped_lock(mutex_);
            if(n_task_ >= max_task_) return -1;
            n_task_++;
        }
        auto result = run_single_task(task_describe);
        return alive_answer.append(std::move(result));
    }

    int Runner::run_child(const nlohmann::json& config) {
        std::vector<std::string> args_val, envp_val;
        std::string exe_path = config["exe_path"];

        if(config.contains("args")) {
            const auto& args = config.at("args");
            if(not args.is_array()) {
                ERROR_EXIT(OJStatus::INVALID_CONFIG);
            }

            args_val = args;
        }
        if(config.contains("envp")) {
            const auto& envp = config.at("envp");
            if(not envp.is_array()) {
                ERROR_EXIT(OJStatus::INVALID_CONFIG);
            }

            envp_val = envp;
        }

        size_t argc = args_val.size(), envc = envp_val.size();
        std::vector<const char*> args_sys, envp_sys;

        args_sys.reserve(argc+1);
        envp_val.reserve(envc+1);

        for(const auto& arg: args_val) args_sys.emplace_back(arg.c_str());
        for(const auto& env: envp_val) envp_sys.emplace_back(env.c_str());

        args_sys.emplace_back(nullptr);
        envp_sys.emplace_back(nullptr);

        execve(exe_path.c_str(), (char* const*)args_sys.data(), (char* const*)envp_sys.data());

        return -1;
    }

    int Runner::stop_task(int task_id) {
        throw std::logic_error("Function not yet implemented");
    }

} // SandBox