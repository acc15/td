#pragma once

#include <GLFW/glfw3.h>

#include <td/engine/engine.h>

namespace td {

class glfw_engine: public engine {
public:
    glfw_engine(GLFWwindow* window, const std::vector<std::string>& args);

private:
    GLFWwindow* _window;
    const std::vector<std::string>& _args;
    friend td::engine;
};


}