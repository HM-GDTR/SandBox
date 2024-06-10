cmake_minimum_required(VERSION 3.26)
project(SandBox VERSION 0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SECCOMP REQUIRED libseccomp)

add_executable(SandBox
        src/main.cpp
        src/server/server.cpp
        src/server/WebServer.cpp
        src/tools/SandBox.hpp
        src/runner/Runner.cpp
        src/error/Errors.hpp
        src/restrict/restrict.cpp
        src/restrict/seccomp_rule.cpp
        src/answer/AliveAnswer.hpp
)

target_include_directories(SandBox PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}"
        "${PROJECT_SOURCE_DIR}/src/answer"
        "${PROJECT_SOURCE_DIR}/src/error"
        "${PROJECT_SOURCE_DIR}/src/restrict"
        "${PROJECT_SOURCE_DIR}/src/runner"
        "${PROJECT_SOURCE_DIR}/src/server"
        "${PROJECT_SOURCE_DIR}/src/tools"
        "${PROJECT_SOURCE_DIR}/3rdparty/cpp-httplib"
        "${PROJECT_SOURCE_DIR}/3rdparty/tclap/include"
        "${PROJECT_SOURCE_DIR}/3rdparty/spdlog/include"
        "${PROJECT_SOURCE_DIR}/3rdparty/nlohmann/include"
)

configure_file(SandBoxConfig.h.in SandBoxConfig.h)
target_link_libraries(SandBox ${SECCOMP_LIBRARIES})
