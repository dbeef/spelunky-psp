//
// Created by dbeef on 2/2/20.
//

#ifndef RESOURCE_COMPILER_DEBUGGLCALL_HPP
#define RESOURCE_COMPILER_DEBUGGLCALL_HPP

#include <cassert>

#include "logger/log.h"
#include "glad/glad.h"

static void GlClearError() {
    while (glGetError() != GL_NO_ERROR);
}

static bool GlLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError())
    {
        log_error("OpenGL error: %i in function: %s, in file: %s, line: %i", error, function, file, line);
        return false;
    }
    return true;
}

#define DebugGlCall(x) \
	GlClearError(); \
	x;\
	assert(GlLogCall(#x, __FILE__, __LINE__))


#endif //RESOURCE_COMPILER_DEBUGGLCALL_HPP
