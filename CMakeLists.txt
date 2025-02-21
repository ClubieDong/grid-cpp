cmake_minimum_required(VERSION 3.27)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable link time optimization
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# Create project
project(grid LANGUAGES CXX)

# Find Python and pybind11
find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)
find_package(pybind11 CONFIG REQUIRED)

# ephem/libastro
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd/ephem)

# Python module
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/grid/*.cpp)
python_add_library(${PROJECT_NAME} MODULE ${SRC_FILES} WITH_SOABI)
target_link_libraries(${PROJECT_NAME} PRIVATE pybind11::headers ephem -static-libstdc++)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/grid)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
install(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_NAME})
