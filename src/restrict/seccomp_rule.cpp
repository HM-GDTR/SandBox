//
// Created by rainbowx on 24-6-10.
//

#include "seccomp_rule.h"

#include <string>

#include <fcntl.h>
#include <sys/stat.h>
#include <spdlog/spdlog.h>

#include "src/error/Errors.hpp"

namespace SandBox {
    int example_seccomp_rule(const nlohmann::json& context, scmp_filter_ctx& ctx) {
        // allow every syscall
        ctx = seccomp_init(SCMP_ACT_ALLOW);

        // use SCMP_ACT_KILL for socket, python will be killed immediately
        if (seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EACCES), SCMP_SYS(socket), 0) != 0) {
            return -1;
        }

        return 0;
    }
    int general_seccomp_rules(const nlohmann::json& context, scmp_filter_ctx& ctx) {
        int syscalls_blacklist[] = {SCMP_SYS(clone),
                                    SCMP_SYS(fork), SCMP_SYS(vfork),
                                    SCMP_SYS(kill), 
    #ifdef __NR_execveat
                                    SCMP_SYS(execveat)
    #endif
                                   };
        int syscalls_blacklist_length = sizeof(syscalls_blacklist) / sizeof(int);
        // load seccomp rules
        ctx = seccomp_init(SCMP_ACT_ALLOW);
        if (!ctx) {
            return -1;
        }
        for (int i = 0; i < syscalls_blacklist_length; i++) {
            if (seccomp_rule_add(ctx, SCMP_ACT_KILL, syscalls_blacklist[i], 0) != 0) {
                return -1;
            }
        }
        // use SCMP_ACT_KILL for socket, python will be killed immediately
        if (seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EACCES), SCMP_SYS(socket), 0) != 0) {
            return -1;
        }
        // add extra rule for execve
        // if (seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(execve), 1, SCMP_A0(SCMP_CMP_NE, (scmp_datum_t)(_config->exe_path))) != 0) {
        //     return -1;
        // }
        // do not allow "w" and "rw" using open
        if (seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(open), 1, SCMP_CMP(1, SCMP_CMP_MASKED_EQ, O_WRONLY, O_WRONLY)) != 0) {
            return -1;
        }
        if (seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(open), 1, SCMP_CMP(1, SCMP_CMP_MASKED_EQ, O_RDWR, O_RDWR)) != 0) {
            return -1;
        }
        // do not allow "w" and "rw" using openat
        if (seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(openat), 1, SCMP_CMP(2, SCMP_CMP_MASKED_EQ, O_WRONLY, O_WRONLY)) != 0) {
            return -1;
        }
        if (seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(openat), 1, SCMP_CMP(2, SCMP_CMP_MASKED_EQ, O_RDWR, O_RDWR)) != 0) {
            return -1;
        }

        if (seccomp_load(ctx) != 0) {
            return -1;
        }
        seccomp_release(ctx);
        return 0;
    }
    int c_cpp_seccomp_rule(const nlohmann::json& context, scmp_filter_ctx& ctx, bool allow_write_file) {
        int syscalls_whitelist[] = {
            SCMP_SYS(access),
            SCMP_SYS(arch_prctl),
            SCMP_SYS(brk),
            SCMP_SYS(clock_gettime),
            SCMP_SYS(close),
            SCMP_SYS(exit_group),
            SCMP_SYS(faccessat),
            SCMP_SYS(fstat),
            SCMP_SYS(futex),
            SCMP_SYS(getrandom),
            SCMP_SYS(lseek),
            SCMP_SYS(mmap),
            SCMP_SYS(mprotect),
            SCMP_SYS(munmap),
            SCMP_SYS(newfstatat),
            SCMP_SYS(pread64),
            SCMP_SYS(prlimit64),
            SCMP_SYS(read),
            SCMP_SYS(readlink),
            SCMP_SYS(readv),
            SCMP_SYS(rseq),
            SCMP_SYS(set_robust_list),
            SCMP_SYS(set_tid_address),
            SCMP_SYS(write),
            SCMP_SYS(writev),
            SCMP_SYS(gettid),
            SCMP_SYS(getpid),
            SCMP_SYS(tgkill),
            SCMP_SYS(get_mempolicy),
        };

        int syscalls_whitelist_length = sizeof(syscalls_whitelist) / sizeof(int);

        // load seccomp rules
        if (!ctx) {
            return -1;
        }

        for (int i = 0; i < syscalls_whitelist_length; i++) {
            if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, syscalls_whitelist[i], 0) != 0) {
                return -1;
            }
        }

        if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(execve), 0) != 0) {
            return -1;
        }

        // extra rule for execve
        if(context.contains("exe_path_ptr")) {
            auto exe_path_ptr = context["exe_path_ptr"];
            std::uintptr_t exe_path_ptr_val = exe_path_ptr;
            if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(execve), 1, SCMP_A0(SCMP_CMP_EQ, (scmp_datum_t)(exe_path_ptr_val))) != 0) {
                return -1;
            }
        }

        if (allow_write_file) {
            if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(open), 0) != 0) {
                return -1;
            }
            if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(openat), 0) != 0) {
                return -1;
            }
            if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(dup), 0) != 0) {
                return -1;
            }
            if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(dup2), 0) != 0) {
                return -1;
            }
            if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(dup3), 0) != 0) {
                return -1;
            }
        } else {
            // do not allow "w" and "rw"
            if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(open), 1, SCMP_CMP(1, SCMP_CMP_MASKED_EQ, O_WRONLY | O_RDWR, 0)) != 0) {
                return -1;
            }
            if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(openat), 1, SCMP_CMP(2, SCMP_CMP_MASKED_EQ, O_WRONLY | O_RDWR, 0)) != 0) {
                return -1;
            }
        }

        return 0;
    }

    SeccompRule::SeccompRule(nlohmann::json context, uint32_t default_act): context(context), ctx(seccomp_init(default_act)) {
        // initial rule
        rule_register(Rule{"general", [context](scmp_filter_ctx& ctx){ return general_seccomp_rules(context, ctx); }});
        rule_register(Rule{"c_cpp", [context](scmp_filter_ctx& ctx){ return c_cpp_seccomp_rule(context, ctx, false); }});
        rule_register(Rule{"c_cpp_io", [context](scmp_filter_ctx& ctx){ return c_cpp_seccomp_rule(context, ctx, true); }});
    }

    int SeccompRule::rule_register(const Rule& rule) {
        if(rule_map.contains(rule.rule_name)) {
            rule_map[rule.rule_name] = rule.rule_descriptor;
        }
        else {
            rule_map.emplace(rule.rule_name, rule.rule_descriptor);
        }
        return 0;
    }

    int SeccompRule::apply_rule(std::string rule_name) {
        if(not rule_map.contains(rule_name)) {
            spdlog::error("Fail to load seccomp rule by name: {}", rule_name);
            return -1;
        }

        auto rule_descriptor = rule_map.at(rule_name);
        if(rule_descriptor(ctx) != 0) {
            spdlog::error("Fail to do seccomp {} rule.", rule_name);
            return -1;
        }

        if (seccomp_load(ctx) != 0) {
            spdlog::error("Fail to load seccomp {} rule.", rule_name);
            return -1;
        }
        seccomp_release(ctx);
        return 0;
    }


} // SandBox