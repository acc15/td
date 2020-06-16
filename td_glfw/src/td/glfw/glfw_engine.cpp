#include <td/glfw/glfw_engine.h>

namespace td {

glfw_engine::glfw_engine(GLFWwindow* window, const std::vector<std::string>& args): _window(window), _args(args) {
}

bool engine::is_key_pressed(int code) const {
    return glfwGetKey(dynamic_cast<const glfw_engine*>(this)->_window, code) == GLFW_PRESS;
}

engine* engine::title(const char* title) {
    glfwSetWindowTitle(dynamic_cast<const glfw_engine*>(this)->_window, title);
    return this;
}

const std::vector<std::string>& engine::launch_args() const {
    return dynamic_cast<const glfw_engine*>(this)->_args;
}

}