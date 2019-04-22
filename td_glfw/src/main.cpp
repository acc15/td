#include <td/gl/gl.h>

#include <GLFW/glfw3.h>

#include <iostream>

#include <td/gl/program.h>
#include <td/core/buffer.h>
#include <td/gl/buffer_object.h>
#include <td/gl/vbo_layout.h>

namespace td {

class drawer {
public:
    drawer(program& p): _program(p), _vbo(nullptr), _vbo_layout(nullptr), _ebo(nullptr) {
        glUseProgram(p.id());
    }

    drawer& ebo(ebo& ebo) {
        _ebo = &ebo;
        return *this;
    }

    drawer& vbo(vbo& vbo, const vbo_layout& layout) {
        _vbo = &vbo;
        _vbo_layout = &layout;
        return *this;
    }

    template <typename T>
    drawer& uniform(const char* name, const T& value) {

        glUniform2dv()
        glUniform1d(glGetUniformLocation())
        glUniform2d()

    }

    void draw(GLenum mode) {
        if (_vbo != nullptr) {
            _vbo->bind();
            _vbo_layout->enable();
        }
        if (_ebo != nullptr) {
            _ebo->bind();
        }
        if (_ebo != nullptr) {
            glDrawElements(mode, _ebo->count(), _ebo->type(), nullptr);
        } else if (_vbo != nullptr) {
            glDrawArrays(mode, 0, _vbo->size() / _vbo_layout->stride());
        }
        if (_ebo != nullptr) {
            _ebo->unbind();
        }
        if (_vbo != nullptr) {
            _vbo_layout->disable();
            _vbo->unbind();
        }
    }

private:
    program& _program;
    td::vbo* _vbo;
    const td::vbo_layout* _vbo_layout;
    td::ebo* _ebo;

};

}

td::program p1 = td::program().add(td::shader(GL_VERTEX_SHADER, "")).add(td::shader(GL_FRAGMENT_SHADER, ""));
td::vbo vbo;
td::vbo_layout l = td::vbo_layout().f(3).f(4).f(3);


void init_game() {

    vbo.bind_and_apply(td::buffer() << 1 << 2 << 3, GL_STATIC_DRAW);

}

void draw_game() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    td::drawer(p1)
        //.vbo()
        //.ebo()
        .draw(GL_TRIANGLES);

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

static void glfw_error_callback(int, const char *description) {
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