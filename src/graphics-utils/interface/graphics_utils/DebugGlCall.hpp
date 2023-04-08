//
// Created by dbeef on 2/2/20.
//

#ifndef RESOURCE_COMPILER_DEBUGGLCALL_HPP
#define RESOURCE_COMPILER_DEBUGGLCALL_HPP

#include <cassert>

#include "logger/log.h"
#include "glad/glad.h"

namespace graphics_utils
{
    static void GlClearError()
    {
        while (glGetError() != GL_NO_ERROR);
    }

    static bool GlLogCall(const char *function, const char *file, int line)
    {
        while (GLenum error = glGetError())
        {
            log_error("OpenGL error: %i in function: %s, in file: %s, line: %i", error, function, file, line);
            return false;
        }
        return true;
    }
}

#ifdef NDEBUG
    #define DebugGlCall(x) x;
#else
    #define DebugGlCall(x) \
        	graphics_utils::GlClearError(); \
        	x;\
        	assert(graphics_utils::GlLogCall(#x, __FILE__, __LINE__))
#endif

#endif //RESOURCE_COMPILER_DEBUGGLCALL_HPP
