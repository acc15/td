#include <td/gl/gl.h>

#include <GLFW/glfw3.h>
#include <iostream>

#include <td/glfw/glfw_engine.h>
#include <td/gl/debug.h>

extern void td_init();

void draw_frame() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    td::engine::get()->emit(td::render_event());
}

void process_frame(float duration) {
    td::engine::get()->emit(td::process_event(duration));
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    switch (action) {
        case GLFW_PRESS:
            td::engine::get()->emit(td::key_down_event(scancode));
            break;

        case GLFW_RELEASE:
            td::engine::get()->emit(td::key_up_event(scancode));
            break;

        case GLFW_REPEAT:
            td::engine::get()->emit(td::key_repeat_event(scancode));
            break;

        default:
            break;
    }
}

bool init_glew(const std::vector<std::string>& args) {
    const GLenum glew_ok = glewInit();
    if (glew_ok != GLEW_OK) {
        std::cerr << "Unable to initialize GLEW: " << glewGetErrorString(glew_ok) << std::endl;
        return false;
    }
    if (td::is_gl_debug_enabled(args)) {
        std::cout << "GLEW version: " << glewGetString(GLEW_VERSION) << std::endl
                  << "GLFW version: " << glfwGetVersionString() << std::endl
                  << "OpenGL version: " << glGetString(GL_VERSION) << std::endl
                  << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl
                  << "OpenGL SL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl
                  << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl;
    }
    return true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    td::engine::get()->emit(td::resize_event(width, height));
}

int run_glfw_window(GLFWwindow* window, const std::vector<std::string>& args) {

    glfwMakeContextCurrent(window);
    // glfwSwapInterval( 0 );
    if (!init_glew(args)) {
        return -1;
    }

    try {

        td::glfw_engine e(window, args);
        td::activate_gl_debug();

        e.activation({ td::event_type::KEY_DOWN, td::event_type::KEY_UP, td::event_type::KEY_REPEAT }, [window](bool active) -> void {
            glfwSetKeyCallback(window, active ? key_callback : nullptr);
        });

        td_init();

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        framebuffer_size_callback(window, width, height);

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        glfwShowWindow(window);

        float frame_duration = 0;
        double frame_time = glfwGetTime();
        while (!glfwWindowShouldClose(window)) {

            process_frame(frame_duration);
            draw_frame();

            glfwSwapBuffers(window);
            glfwPollEvents();

            double current_frame_time = glfwGetTime();
            frame_duration = static_cast<float>(current_frame_time - frame_time);
            frame_time = current_frame_time;

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


int run_glfw(const std::vector<std::string>& args) {

    // glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "td", nullptr, nullptr);
    if (!window) {
        return -1;
    }

    int ret = run_glfw_window(window, args);
    glfwDestroyWindow(window);
    return ret;
}

static void glfw_error_callback(int, const char* description) {
    std::cerr << "GLFW error: " << description << std::endl;
}

int main(int argc, char** argv) {

    std::vector<std::string> args(argv, argv + argc);

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        return -1;
    }

    int ret = run_glfw(args);
    glfwTerminate();
    return ret;
}

