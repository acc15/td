project(td_core)


add_library(td_core STATIC
        src/td/core/img.cpp
        include/td/core/img.h
        src/td/gl/shader.cpp
        include/td/gl/shader.h
        src/td/gl/program.cpp
        include/td/gl/program.h
        include/td/core/buffer.h
        include/td/core/color.h
        include/td/gl/buffer_object.h
        src/td/buffer/vbo_layout.cpp
        include/td/gl/gl_type.h src/td/buffer/buffer.cpp)
target_link_libraries(td_core PNG::PNG GLEW::GLEW)
target_include_directories(td_core PUBLIC include)

add_executable(td_core_test test/main.cpp test/td/buffer/buffer.cpp test/color.cpp test/td/buffer/vbo_layout.cpp)
target_link_libraries(td_core_test Catch2::Catch2 td_core)
catch_discover_tests(td_core_test)
