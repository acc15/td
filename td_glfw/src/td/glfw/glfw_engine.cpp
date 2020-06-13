#include <td/glfw/glfw_engine.h>

namespace td {

glfw_engine::glfw_engine(GLFWwindow* window): _window(window) {
}

bool engine::is_key_pressed(int code) const {
    return glfwGetKey(dynamic_cast<const glfw_engine*>(this)->_window, code) == GLFW_PRESS;
}

void engine::title(const char* title) {
    glfwSetWindowTitle(dynamic_cast<const glfw_engine*>(this)->_window, title);
}

}