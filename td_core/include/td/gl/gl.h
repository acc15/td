//
// Created by acc15 on 03.04.19.
//

#pragma once

#if defined(__ANDROID__)
#   include <GLES2/gl2.h>
#elif defined(__linux__) || defined(WIN32)
#   include <GL/glew.h>
#else
#   error "Your platform isnt supported at the moment"
#endif

