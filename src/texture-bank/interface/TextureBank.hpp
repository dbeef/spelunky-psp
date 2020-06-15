#pragma once

#include "TextureType.hpp"
#include "TextureRegion.hpp"

#include <unordered_map>
#include <vector>
#include <cassert>
#include <cstdint>

using TextureID = unsigned int;
const TextureID INVALID_TEXTURE = 0;
const TextureRegion INVALID_REGION = {};

class TextureBank
{
public:

    static TextureBank &instance();
    static void init();
    static void dispose();

    void load_textures();
    void load_texture_regions();

    const TextureRegion& get_region(TextureType t, std::size_t index)
    {
        const auto it = _texture_regions.find(t);

        // So far all texture regions are loaded up-front, because of that
        // each request is expected to be satisfied:
        assert(it != _texture_regions.end());
        assert(it->second.size() > index);

        return it->second[index];
    }

    TextureID get_texture(TextureType t) const
    {
        const auto it = _texture_ids.find(t);
        // So far all textures are loaded up-front, because of that
        // each request is expected to be satisfied:
        assert(it != _texture_ids.end());
        return it->second;
    }

private:

    static TextureBank *_instance;
    std::unordered_map<TextureType, TextureID> _texture_ids;
    std::unordered_map<TextureType, std::vector<TextureRegion>> _texture_regions;
};
