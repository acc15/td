cmake_minimum_required(VERSION 3.13)
project(td)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(ext/Catch2)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/ext/Catch2/contrib")
include(CTest)
include(Catch)

option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
add_subdirectory(ext/glfw)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PNG REQUIRED)

add_subdirectory(td_core)
add_subdirectory(td_glfw)