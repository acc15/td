project(td_core)


add_library(td_core STATIC
        src/td/core/img.cpp
        include/td/core/img.h
        src/td/core/shader.cpp
        include/td/core/shader.h
        src/td/core/program.cpp
        include/td/core/program.h
        include/td/buffer/buffer.h
        include/td/core/color.h
        include/td/buffer/buffer_object.h
        src/td/buffer/buffer_layout.cpp
        include/td/gl/gl_type.h)
target_link_libraries(td_core PNG::PNG GLEW::GLEW)
target_include_directories(td_core PUBLIC include)

add_executable(td_core_test test/main.cpp test/color.cpp)
target_link_libraries(td_core_test Catch2::Catch2 td_core)
catch_discover_tests(td_core_test)
