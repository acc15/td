project(td_core)


add_library(td_core STATIC
    include/td/core/img.h src/td/core/img.cpp
    include/td/shader/shader.h src/td/shader/shader.cpp
    include/td/shader/program.h src/td/shader/program.cpp
    include/td/gl/gl_type.h
    include/td/gl/buffer_object.h
    include/td/gl/vbo_layout.h src/td/gl/vbo_layout.cpp
    include/td/core/buf.h
    include/td/core/color.h
    include/td/engine/event.h
    include/td/engine/engine.h src/td/engine/engine.cpp
    include/td/engine/object.h src/td/engine/object.cpp
    include/td/engine/fwd.h src/td/engine/event.cpp
    include/td/event/event.h
    include/td/event/listener_registry.h src/td/event/listener_registry.cpp
    include/td/event/listener.h src/td/event/listener.cpp
    include/td/event/emitter.h src/td/event/emitter.cpp
    include/td/util/fn_cast.h
)

target_link_libraries(td_core OpenGL::GL OpenGL::GLU PNG::PNG GLEW::GLEW Eigen3::Eigen fmt::fmt )
target_include_directories(td_core PUBLIC include)

add_executable(td_core_test
    test/main.cpp
    test/td/buf/buf.cpp
    test/td/buf/static_buf.cpp
    test/td/gl/vbo_layout.cpp
    test/td/engine/event.cpp
    test/td/engine/engine.cpp
    test/td/shader/program.cpp
    test/td/event/listener.cpp
)

target_link_libraries(td_core_test Catch2::Catch2 td_core)
catch_discover_tests(td_core_test)
