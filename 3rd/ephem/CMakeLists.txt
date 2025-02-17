cmake_minimum_required(VERSION 3.15)

# ========== Project settings ==========
# Use C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
# Enable link time optimization
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
# Set the project name
project(ephem LANGUAGES C)

# ========== Create targets ==========
# Compile all *.c files in the `pyephem/libastro` directory
file(GLOB SRC_FILES ${CMAKE_SOURCE_DIR}/pyephem/libastro/*.c)
file(GLOB DATA_FILES ${CMAKE_SOURCE_DIR}/pyephem/extensions/data/*.c)
add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${DATA_FILES} ${CMAKE_SOURCE_DIR}/pyephem/extensions/dtoa.c)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/pyephem)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/pyephem/libastro)
