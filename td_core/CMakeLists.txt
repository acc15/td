project(td_core)

add_library(td_core STATIC
    include/td/core/img.h src/td/core/img.cpp
    include/td/gl/shader.h src/td/gl/shader.cpp
    include/td/gl/program.h src/td/gl/program.cpp
    include/td/gl/gl_type.h
    include/td/gl/bo.h
    include/td/gl/vbo_layout.h src/td/gl/vbo_layout.cpp
    include/td/core/buf.h
    include/td/core/color.h
    include/td/event/event.h
    include/td/event/listener_registry.h src/td/event/listener_registry.cpp
    include/td/event/listener.h src/td/event/listener.cpp
    include/td/event/emitter.h src/td/event/emitter.cpp
    include/td/util/fn_cast.h
    include/td/engine/engine.h src/td/engine/engine.cpp
    include/td/engine/obj_registry.h src/td/engine/obj_registry.cpp
    include/td/engine/parent_obj.h src/td/engine/parent_obj.cpp
    include/td/engine/obj.h src/td/engine/obj.cpp
    include/td/engine/child_obj.h src/td/engine/child_obj.cpp
    include/td/engine/tag_registry.h src/td/engine/tag_registry.cpp
    include/td/core/assert.h
    include/td/gl/debug.h src/td/gl/debug.cpp
    include/td/gl/enum.h src/td/gl/enum.cpp
    include/td/gl/drawer.h src/td/gl/drawer.cpp
    include/td/gl/sl_var_type.h src/td/gl/sl_var_type.cpp
    include/td/util/plain_arithmetic_container.h
    include/td/gl/sl_var.h src/td/gl/sl_var_ref.cpp
    include/td/gl/sl_var_ref.h
    include/td/gl/sl_applier_common.h
    include/td/gl/sl_uniform_applier.h
    include/td/gl/sl_attribute_applier.h include/td/gl/shader_type.h src/td/gl/shader_type.cpp)

target_link_libraries(td_core OpenGL::GL OpenGL::GLU PNG::PNG GLEW::GLEW Eigen3::Eigen fmt::fmt )
target_include_directories(td_core PUBLIC include)

add_executable(td_core_test
    test/main.cpp
    test/td/core/buf.cpp
    test/td/core/static_buf.cpp
    test/td/gl/vbo_layout.cpp
    test/td/gl/program.cpp
    test/td/gl/sl_var.cpp
    test/td/event/listener.cpp
    test/td/engine/obj.cpp
    test/td/test/call_rec.h
    test/td/util/plain_arithmetic_container.cpp
    test/td/test/call_rec.cpp)

target_link_libraries(td_core_test Catch2::Catch2 td_core)
catch_discover_tests(td_core_test)
