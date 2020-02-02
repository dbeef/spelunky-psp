#include <cassert>

#include "stb/image.h"
#include "logger/log.h"
#include "LevelRenderer.hpp"

namespace
{
    namespace tilesheet
    {
        #include "gfx_cavebg.png.h"
    }

    stbi_uc* buffer_tilesheet()
    {
        stbi_uc *buffer = nullptr;
        int width;
        int height;
        int bits_per_pixel;
        int desiredChannels = 4;

        buffer = stbi_load_from_memory(
                reinterpret_cast<const stbi_uc *>(&tilesheet::data),
                sizeof(tilesheet::data),
                &width,
                &height,
                &bits_per_pixel,
                desiredChannels
        );

        if (buffer == nullptr)
        {
            log_error("Failed to create texture from passed data. Reason: %s", stbi_failure_reason());
            return nullptr;
        }

        log_info("Created texture: %i/%i, %i bpp.", width, height, bits_per_pixel);
        return buffer;
    }

    void dispose_tilesheet(stbi_uc* buffer)
    {
        stbi_image_free(buffer);
    }
}

LevelRenderer* LevelRenderer::_level_renderer = nullptr;

LevelRenderer &LevelRenderer::instance()
{
    assert(_level_renderer);
    return *_level_renderer;
}

void LevelRenderer::init()
{
    assert(!_level_renderer);
    _level_renderer = new LevelRenderer();
}

void LevelRenderer::dispose()
{
    assert(_level_renderer);
    delete _level_renderer;
}

void LevelRenderer::load_textures()
{
    log_info("Loading tilesheet.");

    glGenTextures(1, &_tilesheet);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); // Won't compile on PSP
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);

    // Load image via STB with data from resource compiler:
    auto* buffer = buffer_tilesheet();

    // Upload to GPU:
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 32, 512, 0, GL_RGB, GL_UNSIGNED_SHORT_4_4_4_4, buffer);

    // Don't need the buffer anymore
    dispose_tilesheet(buffer);

    log_info("Done loading tilesheet.");
}
