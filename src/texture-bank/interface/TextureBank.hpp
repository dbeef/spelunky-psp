#pragma once

#include "TextureType.hpp"

#include <glad/glad.h>
#include <unordered_map>
#include <cassert>

using TextureID = GLuint;
const TextureID INVALID_TEXTURE = 0;

class TextureBank
{
public:

    static TextureBank& instance();
    static void init();
    static void dispose();

    void load_textures();
    void dispose_textures();

    TextureID get_texture(TextureType t) const
    {
        const auto it = _textures.find(t);
        // So far all textures are loaded up-front, because of that
        // each request is expected to be satisfied:
        assert(it != _textures.end());
        return it->second;
    }

private:

    static TextureBank* _instance;
    std::unordered_map<TextureType, TextureID> _textures;
};
