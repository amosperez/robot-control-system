cmake_minimum_required(VERSION 3.20)

project(RobotRuntime LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Robot runtime executable -------------------- #
# Build: cmake --build _build --target robot-runtime
# Run: ./_build/robot-runtime
add_executable(
    robot-runtime
    src/main.cpp
    src/robot_runtime.cpp
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

# Robot model tests --------------------------- #
# Build: cmake --build _build --target robot-model-tests
# Run: ./_build/robot-model-tests
add_executable(
    robot-model-tests
    tests/robot_model_tests.cpp
    src/robot_model.cpp
)

target_include_directories(
    robot-model-tests
    PRIVATE
    include
)

target_compile_options(
    robot-model-tests
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

# Robot runtime tests ------------------------- #
# Build: cmake --build _build --target robot-runtime-tests
# Run: ./_build/robot-runtime-tests
add_executable(
    robot-runtime-tests
    tests/robot_runtime_tests.cpp
    src/robot_runtime.cpp
)

target_include_directories(
    robot-runtime-tests
    PRIVATE
    include
)

target_compile_options(
    robot-runtime-tests
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)