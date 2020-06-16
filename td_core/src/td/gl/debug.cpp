#include <iostream>
#include <td/gl/debug.h>
#include <td/gl/enum.h>
#include <td/engine/engine.h>

#ifdef NDEBUG
    #include <algorithm>
#endif

namespace td {

void GLAPIENTRY gl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cerr << "OpenGL [" << gl_enum_string(severity)
        << "]{" << gl_enum_string(source)
        << "}(" << gl_enum_string(type)
        << ")<" << id
        << ">: " << message
        << std::endl;
}

void enable_gl_debug() {

    glEnable( GL_DEBUG_OUTPUT );
    glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
    glDebugMessageCallback(gl_message_callback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

}

void disable_gl_debug() {

    glDisable( GL_DEBUG_OUTPUT );
    glDisable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
    glDebugMessageCallback(nullptr, nullptr);

}

bool is_gl_debug_enabled(const std::vector<std::string>& args) {
#ifndef NDEBUG
    return true;
#else
    return std::find(args.begin(), args.end(), "-gl_debug") != args.end();
#endif
}

void activate_gl_debug() {
    if (is_gl_debug_enabled(td::engine::get()->launch_args())) {
        enable_gl_debug();
    }
}

}