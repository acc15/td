#pragma once

#include <GLFW/glfw3.h>

#include <td/engine/engine.h>

namespace td {

    class glfw_engine: public engine {
    public:
        glfw_engine(GLFWwindow* window);

    private:
        GLFWwindow* _window;
    };

}