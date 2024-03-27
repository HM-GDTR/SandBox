cmake_minimum_required(VERSION 3.26)
project(SandBox)

set(CMAKE_CXX_STANDARD 20)

add_executable(SandBox src/main.cpp)

target_include_directories(SandBox PUBLIC
        "${PROJECT_SOURCE_DIR}"
        "${PROJECT_SOURCE_DIR}/3rdparty/**/include"
)

add_subdirectory("${PROJECT_SOURCE_DIR}/3rdparty/tclap")
