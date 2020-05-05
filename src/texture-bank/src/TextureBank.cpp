#include "TextureBank.hpp"
#include "graphics_utils/CreateTexture.hpp"
#include "graphics_utils/DisposeTexture.hpp"

#include <cstdint>
#include <vector>
#include <cassert>

TextureBank* TextureBank::_instance = nullptr;

namespace
{
    namespace level_tiles_png
    {
        #include "generated/level_tiles.png.hpp"
    }

    struct TextureInput
    {
        const char* data;
        std::size_t size;
        TextureType type;
    };

    std::vector<TextureInput> get_input()
    {
        std::vector<TextureInput> out;
        out.push_back({level_tiles_png::data, sizeof(level_tiles_png::data), TextureType::CAVE_LEVEL_TILES});
        return out;
    }
}

void TextureBank::load_textures()
{
    const auto input = get_input();
    for (const auto& in : input)
    {
        TextureID texture = graphics_utils::create_texture(in.data, in.size);
        assert(texture);
        _textures.emplace(in.type, texture);
    }
}

void TextureBank::dispose_textures()
{
    for (const auto& kv : _textures)
    {
        graphics_utils::dispose_texture(kv.second);
    }
}

TextureBank &TextureBank::instance()
{
    assert(_instance);
    return *_instance;
}

void TextureBank::init()
{
    assert(!_instance);
    _instance = new TextureBank();
}

void TextureBank::dispose()
{
    assert(_instance);
    delete _instance;
}
