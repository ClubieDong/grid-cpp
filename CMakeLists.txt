cmake_minimum_required(VERSION 3.27)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable link time optimization
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# Create project
project(grid LANGUAGES CXX)

# Main executable
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
