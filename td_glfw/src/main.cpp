#include <td/gl/gl.h>

#include <GLFW/glfw3.h>

#include <iostream>

#include <td/gl/program.h>
#include <td/core/buffer.h>
#include <td/gl/buffer_object.h>
#include <td/gl/vbo_layout.h>

void init_game() {

    // vbo.bind_and_apply(td::buffer() << 1 << 2 << 3, GL_STATIC_DRAW);

}

void draw_game() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);


}


bool init_glew() {
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

        init_game();

        while (!glfwWindowShouldClose(window)) {

            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            glViewport(0, 0, width, height);

            draw_game();

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
    GLFWwindow* window = glfwCreateWindow(640, 480, "Simple example", nullptr, nullptr);
    if (!window) {
        return -1;
    }

    int ret = run_glfw_window(window);
    glfwDestroyWindow(window);
    return ret;
}

static void glfw_error_callback(int, const char* description) {
    std::cerr << "GLFW error: " << description << std::endl;
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

