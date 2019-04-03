project(td_glfw)

add_executable(td_glfw src/main.cpp)
target_link_libraries(td_glfw td_core glfw GLEW::GLEW)