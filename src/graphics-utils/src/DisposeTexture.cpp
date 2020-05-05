#include "graphics_utils/DebugGlCall.hpp"
#include "graphics_utils/DisposeTexture.hpp"
#include "logger/log.h"

void graphics_utils::dispose_texture(GLuint textureId)
{
    log_info("Disposing texture.");
    DebugGlCall(glDeleteTextures(1, &textureId));
}
