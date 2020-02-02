#include "graphics_utils/DebugGlCall.hpp"
#include "graphics_utils/CreateTexture.hpp"

#include "logger/log.h"
#include "stb/image.h"

namespace
{
    struct Texture
    {
        stbi_uc *buffer = nullptr;
        int width{};
        int height{};
        int bytes_per_pixel{};
        const int desiredChannels = 3;
    };

    Texture buffer_texture(const char *data, std::size_t size)
    {
        Texture tex{};

        tex.buffer = stbi_load_from_memory(
                reinterpret_cast<const stbi_uc *>(data),
                size,
                &tex.width,
                &tex.height,
                &tex.bytes_per_pixel,
                tex.desiredChannels
        );

        if (tex.buffer == nullptr)
        {
            log_error("Failed to buffer texture from passed data. Reason: %s", stbi_failure_reason());
            return tex;
        }

        log_info("Buffered texture: %i/%i, %i bpp.", tex.width, tex.height, tex.bytes_per_pixel);
        return tex;
    }

    void dispose_buffer(Texture &tex)
    {
        stbi_image_free(tex.buffer);
    }

    // https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
    unsigned int round_to_multiple_of_2(unsigned int v)
    {
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v++;
        return v;
    }
}

GLuint graphics_utils::createTexture(const char *data, std::size_t size)
{
    log_info("Buffering texture.");
    Texture tex = buffer_texture(data, size);
    if (!tex.buffer) return 0;

    GLuint texture_id = 0;

    DebugGlCall(glGenTextures(1, &texture_id));
    DebugGlCall(glBindTexture(GL_TEXTURE_2D, texture_id));

    DebugGlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    DebugGlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

    // Upload to GPU rounded to the next power of 2, because of multiplicity constraints of early OpenGL):

    int roundedWidth = round_to_multiple_of_2(tex.width);
    int roundedHeight = round_to_multiple_of_2(tex.height);

    log_info("Rounding texture width: %i to %i, height: %i to %i",
             tex.width, roundedWidth, tex.height, roundedHeight);
    log_info("Uploading texture.");

    DebugGlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, roundedWidth, roundedHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tex.buffer));

    // Don't need the buffer anymore:
    dispose_buffer(tex);

    log_info("Uploaded texture successfuly, texture id: %i", texture_id);
    return texture_id;
}