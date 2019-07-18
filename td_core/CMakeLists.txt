project(td_core)


add_library(td_core STATIC
        include/td/core/img.h src/td/core/img.cpp
        include/td/gl/shader.h src/td/gl/shader.cpp
        include/td/gl/program.h src/td/gl/program.cpp
        include/td/gl/vbo_layout.h src/td/gl/vbo_layout.cpp
        include/td/core/buffer.h
        include/td/core/color.h
        include/td/gl/gl_type.h
        include/td/gl/buffer_object.h
)

target_link_libraries(td_core PNG::PNG GLEW::GLEW Eigen3::Eigen)
target_include_directories(td_core PUBLIC include)

add_executable(td_core_test test/main.cpp test/td/buffer/buffer.cpp test/color.cpp test/td/gl/vbo_layout.cpp)
target_link_libraries(td_core_test Catch2::Catch2 td_core)
catch_discover_tests(td_core_test)
