//
// Created by RainbowX on 24-4-10.
//

#ifndef SANDBOX_BASEEXCEPTION_H
#define SANDBOX_BASEEXCEPTION_H

#include <exception>
#include <source_location>

#if __has_include("stacktrace")
#include <stacktrace>
using std::stacktrace;
#define INIT_TRACE(trace) stacktrace trace = stacktrace::current()
#define TRACE_TO_STR(trace) std::to_string(trace)

#else

#include <boost/stacktrace.hpp>
using boost::stacktrace::stacktrace;
#define INIT_TRACE(trace) stacktrace trace = {}
#define TRACE_TO_STR(trace) boost::stacktrace::to_string(trace)

#endif

#include "ThreadInfo.hpp"

namespace SandBox {

    class BaseException : std::exception {
    private:
        pid_t pid;
        std::string msg;
        stacktrace trace;
    public:
        explicit BaseException(pid_t pid = gettid(), INIT_TRACE(trace));
        explicit BaseException(const char* msg, pid_t pid = gettid(), INIT_TRACE(trace));
        explicit BaseException(std::string msg, pid_t pid = gettid(), INIT_TRACE(trace));
        explicit BaseException(std::string_view msg, pid_t pid = gettid(), INIT_TRACE(trace));

        [[nodiscard]] const char *what() const noexcept override;

        [[nodiscard]] virtual std::string to_string() const noexcept;

        ~BaseException() noexcept override = default;
    };

} // SandBox

#endif //SANDBOX_BASEEXCEPTION_H
