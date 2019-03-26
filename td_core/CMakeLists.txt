cmake_minimum_required(VERSION 3.13)
project(td_core)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
add_library(td_core main.cpp)

target_link_libraries(td_core ${OPENGL_LIBRARY})