//
// Created by RainbowX on 24-4-10.
//

#include "BaseException.h"

#include <utility>

namespace SandBox {
    BaseException::BaseException(pid_t pid, stacktrace trace):
            trace(std::move(trace)),
            std::exception(),
            pid(pid),
            msg() {}

    BaseException::BaseException(const char* msg, pid_t pid, stacktrace trace):
            trace(std::move(trace)),
            std::exception(),
            pid(pid),
            msg(msg) {}

    BaseException::BaseException(std::string msg, pid_t pid, stacktrace trace):
            trace(std::move(trace)),
            std::exception(),
            pid(pid),
            msg(std::move(msg)) {}

    BaseException::BaseException(std::string_view msg, pid_t pid, stacktrace trace):
            trace(std::move(trace)),
            std::exception(),
            pid(pid),
            msg(msg) {}

    const char * BaseException::what() const noexcept {
        return msg.c_str();
    }

    std::string BaseException::to_string() const noexcept {
        std::string ret = "Exception in thread \"";
        ret += threadInfo.getThreadName(pid)+'\"';
        if(not msg.empty()) {
            ret += " detail: "+ msg;
        }
        ret += " at:\n"+ TRACE_TO_STR(trace);
        return ret;
    }

} // SandBox