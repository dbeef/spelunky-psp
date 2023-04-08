//
// Created by dbeef on 2/2/20.
//

#ifndef RESOURCE_COMPILER_CREATETEXTURE_HPP
#define RESOURCE_COMPILER_CREATETEXTURE_HPP

#include <cstdlib>
#include "glad/glad.h"

namespace graphics_utils
{
    GLuint create_texture(const char *data, std::size_t size);
}

#endif //RESOURCE_COMPILER_CREATETEXTURE_HPP
