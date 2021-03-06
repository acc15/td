cmake_minimum_required(VERSION 3.13)

cmake_policy(SET CMP0002 OLD)

project(td)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-error=unused-variable -Wno-error=unused-but-set-variable")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    message(STATUS "MSVC detected, adding compile flags")
    SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /WX")
endif()

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

# OpenGL
find_package(OpenGL REQUIRED)

# GLEW
set(GLEW_VERBOSE TRUE)
set(GLEW_USE_STATIC_LIBS ${WIN32})
find_package(GLEW REQUIRED)

# zlib
add_subdirectory(ext/zlib EXCLUDE_FROM_ALL)

# libpng
option(PNG_SHARED OFF)
option(PNG_EXECUTABLES OFF)
option(PNG_TESTS OFF)
add_subdirectory(ext/libpng EXCLUDE_FROM_ALL)

# libjpeg-turbo
#option(WITH_SIMD OFF)
#option(ENABLE_SHARED OFF)
#set(CMAKE_INSTALL_DOCDIR "${CMAKE_BINARY_DIR}/libjpeg-turbo")
#add_subdirectory(ext/libjpeg-turbo EXCLUDE_FROM_ALL)

# Eigen
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# Sub-projects
add_subdirectory(td_rc)
add_subdirectory(td_core)
add_subdirectory(td_glfw)

if (TD_BUILD_EXAMPLES)
    add_subdirectory(examples/pong)
endif()
