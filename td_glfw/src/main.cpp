#include <td/core/gl.h>

#include <GLFW/glfw3.h>

#include <iostream>

#include <td/core/program.h>

static void glfw_error_callback(int, const char *description) {
    std::cerr << "GLFW error: " << description << std::endl;
}

void draw_game(size_t width, size_t height) {
    glViewport(0, 0, width, height);
    glClearColor(0, 0.2, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void init_game() {

    std::cout << "Using OpenGL v" << glGetString(GL_VERSION) << std::endl;

    td::program p = td::program()
            .vertex(R"(
#version 330 core
layout (location = 0) in vec3 aPos;
out vec4 vertexColor;
void main()
{
    gl_Position = vec4(aPos, 1.0);
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0);
}
)").fragment(R"(
#version 330 core
out vec4 FragColor;
in vec4 vertexColor;
void main()
{
    FragColor = vertexColor;
}
)");


    std::cout << "program id is " << p.id() << std::endl;

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
            draw_game(width, height);

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