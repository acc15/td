#pragma once

#if defined(__ANDROID__)

    #include <GLES2/gl2.h>

#elif defined(__linux__) || defined(WIN32)

    #define GLEW_STATIC
    #include <GL/glew.h>

#else

    #error "Your platform isn't supported at the moment"

#endif

