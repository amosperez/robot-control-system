cmake_minimum_required(VERSION 3.20)

project(RobotRuntime LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(
    robot-runtime
    src/main.cpp
    src/robot_model.cpp
)

target_include_directories(
    robot-runtime
    PRIVATE
    include
)

target_compile_options(
    robot-runtime
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

add_executable(
    robot-runtime-tests
    tests/robot_model_tests.cpp
    src/robot_model.cpp
)

target_include_directories(
    robot-runtime-tests
    PRIVATE
    include
)