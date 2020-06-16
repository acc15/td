#include <iostream>
#include <unordered_map>
#include <td/gl/debug.h>
#include <td/engine/engine.h>

#ifdef NDEBUG
    #include <algorithm>
#endif

#include <td/gl/gl.h>

namespace td {

std::unordered_map<GLenum, const char*> debug_source_map = {
    { GL_DEBUG_SOURCE_API, "API" },
    { GL_DEBUG_SOURCE_WINDOW_SYSTEM, "WINDOW_SYSTEM" },
    { GL_DEBUG_SOURCE_SHADER_COMPILER, "SHADER_COMPILER" },
    { GL_DEBUG_SOURCE_THIRD_PARTY, "THIRD_PARTY" },
    { GL_DEBUG_SOURCE_APPLICATION, "APPLICATION" },
    { GL_DEBUG_SOURCE_OTHER, "OTHER" }
};

std::unordered_map<GLenum, const char*> debug_type_map = {
    { GL_DEBUG_TYPE_ERROR, "ERROR" },
    { GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "DEPRECATED_BEHAVIOR" },
    { GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "UNDEFINED_BEHAVIOR" },
    { GL_DEBUG_TYPE_PORTABILITY, "PORTABILITY" },
    { GL_DEBUG_TYPE_PERFORMANCE, "PERFORMANCE" },
    { GL_DEBUG_TYPE_MARKER, "MARKER" },
    { GL_DEBUG_TYPE_PUSH_GROUP, "PUSH_GROUP" },
    { GL_DEBUG_TYPE_POP_GROUP, "POP_GROUP" },
    { GL_DEBUG_TYPE_OTHER, "OTHER" },
};

std::unordered_map<GLenum, const char*> debug_severity_map = {
    { GL_DEBUG_SEVERITY_HIGH, "HIGH" },
    { GL_DEBUG_SEVERITY_MEDIUM, "MEDIUM" },
    { GL_DEBUG_SEVERITY_LOW, "LOW" },
    { GL_DEBUG_SEVERITY_NOTIFICATION, "NOTIFICATION" }
};

const char* enum_value(GLenum value, const std::unordered_map<GLenum, const char*>& map) {
    auto iter = map.find(value);
    if (iter == map.end()) {
        return "(UNKNOWN)";
    }
    return iter->second;
}


void GLAPIENTRY gl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cerr << "OpenGL [" << enum_value(severity, debug_severity_map)
        << "]{" << enum_value(source, debug_source_map)
        << "}(" << enum_value(type, debug_type_map)
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

bool is_gl_debug_enabled(const std::vector<std::string> args) {
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