//
// Created by rainbowx on 24-6-9.
//

#ifndef SANDBOX_HPP
#define SANDBOX_HPP

#include <cassert>

#include "boost/test/debug.hpp"

namespace SandBox {
    constexpr void _debugbreak() {
        __asm__("int $3");
    }

    #if CMAKE_BUILD_TYPE == Debug
    #define IS_DEBUG "cmake"
    #endif // CMAKE_BUILD_TYPE == Debug

    #define MYCALL
    #ifdef IS_DEBUG
    #define Crash(...) (_debugbreak(),0)
    #define Debug(...) (_debugbreak(),0)

    #define ensure(expression) (void)(!!(expression)||Crash())
    #define static_ensure(expression) static_assert(expression)

    #else
    #define Crash(...) (exit(-1),0)
    #define Debug(...) ((void)0)

    #define ensure(expression) assert(expression)
    #define static_ensure(expression) static_assert(expression)
    #endif // IS_DEBUG

}

#endif //SANDBOX_HPP
