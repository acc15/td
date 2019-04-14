#include <td/core/gl.h>

#include <GLFW/glfw3.h>

#include <iostream>

#include <td/core/img.h>
#include <td/core/shader.h>
#include <td/core/color.h>

static void glfw_error_callback(int, const char *description) {
    std::cerr << "GLFW error: " << description << std::endl;
}

bool init_glew() {

    std::cout << td::type_color_component_limits<float>::max() << std::endl;
    std::cout << td::type_color_component_limits<int>::max() << std::endl;


    const GLenum glew_ok = glewInit();
    if (glew_ok != GLEW_OK) {
        std::cerr << "Unable to initialize GLEW: " << glewGetErrorString(glew_ok) << std::endl;
        return false;
    }
    std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    return true;
}

int run_glfw_window(GLFWwindow* window) {

    glfwMakeContextCurrent(window);
    if (!init_glew()) {
        return -1;
    }

    try {

        td::shader s(GL_VERTEX_SHADER, "uint8 i;");

        GLuint shader_id = s.id();
        std::cout << "shader id is " << shader_id << std::endl;

        while (!glfwWindowShouldClose(window)) {

            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            glViewport(0, 0, width, height);
            glClearColor(0, 0.2, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

    } catch (const std::exception& e) {
        std::cerr << "uncaught exception: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "unexpected unknown exception" << std::endl;
        return -1;
    }

    return 0;
}


int run_glfw() {
    GLFWwindow* window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window) {
        return -1;
    }

    int ret = run_glfw_window(window);
    glfwDestroyWindow(window);
    return ret;
}

int main() {
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        return -1;
    }

    int ret = run_glfw();
    glfwTerminate();
    return ret;
}