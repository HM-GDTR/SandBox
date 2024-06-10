//
// Created by rainbowx on 24-6-10.
//

#include "restrict.h"

#include <grp.h>
#include <csignal>
#include <unistd.h>
#include <spdlog/spdlog.h>

#include <vector>
#include <string>

#include <sys/resource.h>
#include "seccomp_rule.h"

#include "Errors.hpp"

namespace SandBox {
    int do_restricts(const nlohmann::json& config) {
        // memory limit (contains stack and heep)
        if(config.contains("max_stack")) {
            const auto& max_stack = config.at("max_stack");
            if(not max_stack.is_number_unsigned()) {
                ERROR_EXIT(OJStatus::INVALID_CONFIG);
            }
            rlimit stack_limit{};
            size_t max_stack_val = max_stack;
            stack_limit.rlim_cur = stack_limit.rlim_max = max_stack_val;
            if (setrlimit(RLIMIT_STACK, &stack_limit) != 0) {
                ERROR_EXIT(OJStatus::SETRLIMIT_FAILED);
            }
        }
        if(config.contains("max_heap")) {
            const auto& max_heap = config.at("max_heap");
            if(not max_heap.is_number_unsigned()) {
                ERROR_EXIT(OJStatus::INVALID_CONFIG);
            }
            rlimit heap_limit{};
            size_t max_heap_val = max_heap;
            heap_limit.rlim_cur = heap_limit.rlim_max = max_heap_val;

            // RLIMIT_DATA contains heap and .text section
            if (setrlimit(RLIMIT_DATA, &heap_limit) != 0) {
                ERROR_EXIT(OJStatus::SETRLIMIT_FAILED);
            }
        }

        // time limit (contains cpu time only, real time was restricted by parent process)
        if(config.contains("max_cpu_time")) {
            const auto& max_cpu_time = config.at("max_cpu_time");
            if(not max_cpu_time.is_number_unsigned()) {
                ERROR_EXIT(OJStatus::INVALID_CONFIG);
            }
            rlimit cpu_time_limit{};
            size_t max_cpu_time_val = max_cpu_time;
            cpu_time_limit.rlim_cur = cpu_time_limit.rlim_max = max_cpu_time_val;

            // only cpu time was restricted, not real time
            if (setrlimit(RLIMIT_CPU, &cpu_time_limit) != 0) {
                ERROR_EXIT(OJStatus::SETRLIMIT_FAILED);
            }
        }

        // max process number limit
        if(config.contains("max_process")) {
            const auto& max_process = config.at("max_process");
            if(not max_process.is_number_unsigned()) {
                ERROR_EXIT(OJStatus::INVALID_CONFIG);
            }
            rlimit process_limit{};
            size_t max_process_val = max_process;
            process_limit.rlim_cur = process_limit.rlim_max = max_process_val;

            if (setrlimit(RLIMIT_NPROC, &process_limit) != 0) {
                ERROR_EXIT(OJStatus::SETRLIMIT_FAILED);
            }
        }

        // max output size limit
        if(config.contains("max_output")) {
            const auto& max_output = config.at("max_output");
            if(not max_output.is_number_unsigned()) {
                ERROR_EXIT(OJStatus::INVALID_CONFIG);
            }
            rlimit output_limit{};
            size_t max_output_val = max_output;
            output_limit.rlim_cur = output_limit.rlim_max = max_output_val;

            if (setrlimit(RLIMIT_FSIZE, &output_limit) != 0) {
                ERROR_EXIT(OJStatus::SETRLIMIT_FAILED);
            }
        }

        // set user group( uid, gid etc. )
        if(config.contains("run_gid")) {
            const auto& run_gid = config.at("run_gid");
            if(not run_gid.is_array()) {
                ERROR_EXIT(OJStatus::INVALID_CONFIG);
            }

            std::vector<gid_t> run_gid_val = run_gid;

            if(setgid(run_gid_val[0]) == -1) {
                ERROR_EXIT(OJStatus::SETUID_FAILED);
            }

            if(setgroups(run_gid_val.size(), run_gid_val.data()) == -1) {
                ERROR_EXIT(OJStatus::SETUID_FAILED);
            }
        }
        if(config.contains("run_uid")) {
            const auto& run_uid = config.at("run_uid");
            if(not run_uid.is_number_unsigned()) {
                ERROR_EXIT(OJStatus::INVALID_CONFIG);
            }

            uid_t run_uid_val = run_uid;

            if(setuid(run_uid_val) == -1) {
                ERROR_EXIT(OJStatus::SETUID_FAILED);
            }
        }

        // redirect std stream(stdin, stdout, stderr)
        (void)(not config.contains("input_file_path") && (spdlog::warn("Input file is not specified"), true));
        if(config.contains("input_file_path")) {
            const auto& input_file_path = config.at("input_file_path");
            if(not input_file_path.is_string()) {
                ERROR_EXIT(OJStatus::INVALID_CONFIG);
            }

            std::string input_file_path_str = input_file_path;

            freopen(input_file_path_str.c_str(), "r", stdin);
        }
        (void)(not config.contains("output_file_path") && (spdlog::warn("Output file is not specified"), true));
        if(config.contains("output_file_path")) {
            const auto& output_file_path = config.at("output_file_path");
            if(not output_file_path.is_string()) {
                ERROR_EXIT(OJStatus::INVALID_CONFIG);
            }

            std::string output_file_path_str = output_file_path;

            freopen(output_file_path_str.c_str(), "w", stdin);
        }
        (void)(not config.contains("error_file_path") && (spdlog::warn("Error file is not specified"), true));
        if(config.contains("error_file_path")) {
            const auto& error_file_path = config.at("error_file_path");
            if(not error_file_path.is_string()) {
                ERROR_EXIT(OJStatus::INVALID_CONFIG);
            }

            std::string error_file_path_str = error_file_path;

            freopen(error_file_path_str.c_str(), "w", stdin);
        }

        //config["test"] = nlohmann::detail::value_t::null
        // load seccomp rules
        if(config.contains("seccomp_template")) {
            const auto& seccomp_template = config.at("seccomp_template");
            if(not seccomp_template.is_string()) {
                ERROR_EXIT(OJStatus::INVALID_CONFIG);
            }

            SeccompRule rules{};
            std::string seccomp_template_str = seccomp_template;
            if(rules.apply_rule(seccomp_template_str) != 0) {
                ERROR_EXIT(OJStatus::LOAD_SECCOMP_FAILED);
            }
        }

        return 0;
    }

} // SandBox