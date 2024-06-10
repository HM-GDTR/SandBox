//
// Created by rainbowx on 24-6-9.
//

#ifndef ERRORS_HPP
#define ERRORS_HPP

namespace SandBox {

/**
 * 代码运行状态
 */
enum class RunStatus {
    NoError = 0,

    // 编译阶段的错误
    CompilationError,

    // 运行阶段的错误
    RuntimeError,
    TimeLimitExceedError,
    MemoryLimitExceedError,
    RestrictedError,    //调用了不该调用的函数

    // 验证阶段的错误
    WrongAnswerError,
    PresentationError,
    OutputExceedError,

    // 内部错误
    InternalError
};

enum class OJStatus {
    SUCCESS = 0,
    INVALID_CONFIG = -1,
    FORK_FAILED = -2,
    PTHREAD_FAILED = -3,
    WAIT_FAILED = -4,
    ROOT_REQUIRED = -5,
    LOAD_SECCOMP_FAILED = -6,
    SETRLIMIT_FAILED = -7,
    DUP2_FAILED = -8,
    SETUID_FAILED = -9,
    EXECVE_FAILED = -10,
    SPJ_ERROR = -11
};

constexpr const char * to_string(RunStatus e) {
    switch (e) {
        case RunStatus::NoError                    : return "NoError";
        case RunStatus::CompilationError           : return "CompilationError";
        case RunStatus::RuntimeError               : return "RuntimeError";
        case RunStatus::TimeLimitExceedError       : return "TimeLimitExceedError";
        case RunStatus::MemoryLimitExceedError     : return "MemoryLimitExceedError";
        case RunStatus::RestrictedError            : return "RestrictedError";
        case RunStatus::WrongAnswerError           : return "WrongAnswerError";
        case RunStatus::PresentationError          : return "PresentationError";
        case RunStatus::OutputExceedError          : return "OutputExceedError";
        case RunStatus::InternalError              : return "InternalError";
        default                                     : return "Unknown";
    }
}

constexpr int to_int(RunStatus e) {
    switch (e) {
        case RunStatus::NoError                    : return 0           ;
        case RunStatus::CompilationError           : return 1           ;
        case RunStatus::RuntimeError               : return 2           ;
        case RunStatus::TimeLimitExceedError       : return 3           ;
        case RunStatus::MemoryLimitExceedError     : return 4           ;
        case RunStatus::RestrictedError            : return 5           ;
        case RunStatus::WrongAnswerError           : return 6           ;
        case RunStatus::PresentationError          : return 7           ;
        case RunStatus::OutputExceedError          : return 8           ;
        case RunStatus::InternalError              : return 9           ;
        default                                    : return -1          ;
    }
}

constexpr RunStatus from_int(int val) {
    switch (val) {
        case 0 : return RunStatus::NoError               ;
        case 1 : return RunStatus::CompilationError      ;
        case 2 : return RunStatus::RuntimeError          ;
        case 3 : return RunStatus::TimeLimitExceedError  ;
        case 4 : return RunStatus::MemoryLimitExceedError;
        case 5 : return RunStatus::RestrictedError       ;
        case 6 : return RunStatus::WrongAnswerError      ;
        case 7 : return RunStatus::PresentationError     ;
        case 8 : return RunStatus::OutputExceedError     ;
        case 9 : return RunStatus::InternalError         ;
        default: return RunStatus::InternalError         ;
    }
}

} // SandBox

#endif //ERRORS_HPP
