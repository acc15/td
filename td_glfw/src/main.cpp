#include <td/gl/gl.h>

#include <GLFW/glfw3.h>
#include <iostream>

#include <td/glfw/glfw_engine.h>

extern void td_init();

void draw_frame() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    td::engine::get().emit(td::render_event());
}

void process_frame() {
    td::engine::get().emit(td::process_event());
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    switch (action) {
        case GLFW_PRESS:
            td::engine::get().emit(td::key_down_event(scancode));
            break;

        case GLFW_RELEASE:
            td::engine::get().emit(td::key_up_event(scancode));
            break;

        case GLFW_REPEAT:
            td::engine::get().emit(td::key_repeat_event(scancode));
            break;

        default:
            break;
    }
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

        td::glfw_engine e(window);
        e.activation({ td::event_type::KEY_DOWN, td::event_type::KEY_UP, td::event_type::KEY_REPEAT }, [window](bool active) -> void {
            glfwSetKeyCallback(window, active ? key_callback : nullptr);
        });

        td_init();

        while (!glfwWindowShouldClose(window)) {

            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            glViewport(0, 0, width, height);

            draw_frame();
            process_frame();

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

