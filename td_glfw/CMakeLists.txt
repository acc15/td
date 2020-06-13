project(td_glfw)

add_library(td_glfw src/main.cpp include/td/glfw/glfw_engine.h src/td/glfw/glfw_engine.cpp)
target_include_directories(td_glfw PRIVATE include)
target_link_libraries(td_glfw td_core glfw)
