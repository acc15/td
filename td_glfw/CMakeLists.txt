project(td_glfw)


#add_custom_command(
#        OUTPUT generated/res.cpp generated/res.hpp
#        DEPENDS res/my_shader.vs
#        COMMAND td_rc -id ${CMAKE_CURRENT_LIST_DIR} -i res/my_shader.vs -h generated/res.hpp -c generated/res.cpp)

add_executable(td_glfw generated/res.cpp generated/res.hpp src/main.cpp)

target_include_directories(td_glfw PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated)
target_link_libraries(td_glfw td_core glfw OpenGL::GL GLEW::GLEW)