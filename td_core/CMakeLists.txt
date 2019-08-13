project(td_core)


add_library(td_core STATIC
        include/td/core/img.h src/td/core/img.cpp
        include/td/shader/shader.h src/td/shader/shader.cpp
        include/td/shader/program.h src/td/shader/program.cpp
        include/td/gl/gl_type.h
        include/td/gl/buffer_object.h
        include/td/gl/vbo_layout.h src/td/gl/vbo_layout.cpp
        include/td/core/buffer.h
        include/td/core/color.h
        include/td/engine/event.h include/td/engine/engine.h include/td/engine/object.h src/td/engine/engine.cpp include/td/engine/fwd.h src/td/engine/event.cpp src/td/engine/object.cpp)

target_link_libraries(td_core PNG::PNG GLEW::GLEW Eigen3::Eigen)
include_directories(td_core include)

add_executable(td_core_test test/main.cpp test/td/buffer/buffer.cpp test/color.cpp test/td/gl/vbo_layout.cpp)
target_link_libraries(td_core_test Catch2::Catch2 td_core)
catch_discover_tests(td_core_test)
