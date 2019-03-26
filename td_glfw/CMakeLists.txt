cmake_minimum_required(VERSION 3.13)
project(glfw)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

add_subdirectory(ext/glfw)
include_directories(ext/glfw/include)

add_executable(td_glfw src/main.cpp)

target_link_libraries(td_glfw glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARY})