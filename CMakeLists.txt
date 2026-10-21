cmake_minimum_required(VERSION 3.10)
project(image_processor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(image_processor ${SOURCES})

target_include_directories(image_processor PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_compile_options(image_processor PRIVATE -Wall -Wextra -Wpedantic)