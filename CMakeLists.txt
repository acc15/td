cmake_minimum_required(VERSION 3.13)
project(td)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-error=unused-variable -Wno-error=unused-but-set-variable")
endif(CMAKE_COMPILER_IS_GNUCXX)

# Catch
add_subdirectory(ext/Catch2 EXCLUDE_FROM_ALL)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/ext/Catch2/contrib")
include(CTest)
include(Catch)

# GLFW
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
add_subdirectory(ext/glfw EXCLUDE_FROM_ALL)

# {fmt}
add_subdirectory(ext/fmt EXCLUDE_FROM_ALL)

find_package(OpenGL REQUIRED)

set(GLEW_VERBOSE TRUE)
set(GLEW_USE_STATIC_LIBS TRUE)
find_package(GLEW REQUIRED)

find_package(PNG REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

add_subdirectory(td_rc)
add_subdirectory(td_core)
add_subdirectory(td_glfw)
