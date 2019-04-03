project(td_core)

add_library(td_core STATIC src/td/core/img.cpp src/td/core/shader.cpp src/td/core/exception.cpp include/td/core/fmt.h)
target_link_libraries(td_core OpenGL::GL PNG::PNG)
target_include_directories(td_core PUBLIC include)

add_executable(td_core_test test/main.cpp)
target_link_libraries(td_core_test Catch2::Catch2 td_core GLEW::GLEW)
catch_discover_tests(td_core_test)
