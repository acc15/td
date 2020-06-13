project(td_glfw)

add_library(td_glfw src/main.cpp)
target_link_libraries(td_glfw td_core glfw)
