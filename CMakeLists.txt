cmake_minimum_required(VERSION 3.26)
project(SandBox VERSION 0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_executable(SandBox
        src/main.cpp
        src/runner/Runner.cpp
        src/include/types.hpp
        src/runner/Worker.cpp
        src/include/status.hpp
        src/restrict/restrict.cpp
        src/server/Server.cpp
        src/include/ThreadInfo.hpp
        src/exception/BaseException.cpp
        src/include/BaseException.h
)

target_include_directories(SandBox PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}"
        "${PROJECT_SOURCE_DIR}/src/include"
        "${PROJECT_SOURCE_DIR}/3rdparty/tclap/include"
        "${PROJECT_SOURCE_DIR}/3rdparty/spdlog/include"
        "${PROJECT_SOURCE_DIR}/3rdparty/nlohmann/include"
        "${PROJECT_SOURCE_DIR}/3rdparty/boost/asio/include"
)

target_link_libraries(SandBox PUBLIC backtrace)

configure_file(SandBoxConfig.h.in SandBoxConfig.h)
