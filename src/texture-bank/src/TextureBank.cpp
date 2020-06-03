#include "TextureBank.hpp"
#include "graphics_utils/CreateTexture.hpp"
#include "logger/log.h"
#include "cJSON.h"

#include <cstdint>
#include <limits>
#include <vector>
#include <cassert>
#include <stdexcept>

TextureBank *TextureBank::_instance = nullptr;

namespace
{
    namespace level_tiles_png
    {
        #include "generated/level_tiles.png.hpp"
    }

    namespace level_tiles_json
    {
        #include "generated/level_tiles.json.hpp"
    }

    namespace main_dude_png
    {
        #include "generated/main-dude-spritesheet.png.hpp"
    }

    namespace main_dude_json
    {
        #include "generated/main-dude-spritesheet.json.hpp"
    }

    namespace main_menu_png
    {
        #include "generated/main-menu-spritesheet.png.hpp"
    }

    namespace main_menu_json
    {
        #include "generated/main-menu-spritesheet.json.hpp"
    }

    namespace hud_png
    {
        #include "generated/hud.png.hpp"
    }

    namespace hud_json
    {
        #include "generated/hud.json.hpp"
    }

    struct TextureInput
    {
        const char *data;
        std::size_t size;
        TextureType type;
    };

    struct TextureRegionInput
    {
        const char *data;
        TextureType type;
    };

    std::vector<TextureInput> get_texture_input()
    {
        std::vector<TextureInput> out;
        out.push_back({level_tiles_png::data, sizeof(level_tiles_png::data), TextureType::CAVE_LEVEL_TILES});
        out.push_back({main_dude_png::data, sizeof(main_dude_png::data), TextureType::MAIN_DUDE});
        out.push_back({main_menu_png::data, sizeof(main_menu_png::data), TextureType::MAIN_MENU});
        out.push_back({hud_png::data, sizeof(hud_png::data), TextureType::HUD});
        return out;
    }

    std::vector<TextureRegionInput> get_texture_region_input()
    {
        std::vector<TextureRegionInput> out;
        out.push_back({level_tiles_json::data, TextureType::CAVE_LEVEL_TILES});
        out.push_back({main_dude_json::data, TextureType::MAIN_DUDE});
        out.push_back({main_menu_json::data, TextureType::MAIN_MENU});
        out.push_back({hud_json::data, TextureType::HUD});
        return out;
    }
}

void TextureBank::load_textures()
{
    const auto input = get_texture_input();
    for (const auto &in : input)
    {
        TextureID texture = graphics_utils::create_texture(in.data, in.size);
        assert(texture);
        _texture_ids.emplace(in.type, texture);
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

void TextureBank::load_texture_regions()
{
    const auto input = get_texture_region_input();
    for (const auto &in : input)
    {
        std::vector<TextureRegion> regions;
        try
        {
            cJSON *document_root = cJSON_Parse(in.data);
            if (!document_root)
            {
                const char *error_ptr = cJSON_GetErrorPtr();
                if (error_ptr != nullptr)
                {
                    throw std::runtime_error(std::string(error_ptr));
                } else
                {
                    throw std::runtime_error("Failed to parse document root.");
                }
            }

            std::string filename;
            cJSON *filename_document = cJSON_GetObjectItemCaseSensitive(document_root, "image");
            assert(cJSON_IsString(filename_document) && (filename_document->valuestring != nullptr));
            filename = filename_document->valuestring;

            uint16_t image_width = 0;
            cJSON *image_width_document = cJSON_GetObjectItemCaseSensitive(document_root, "image_width");
            assert(cJSON_IsNumber(image_width_document));
            image_width = image_width_document->valueint;

            uint16_t image_height = 0;
            cJSON *image_height_document = cJSON_GetObjectItemCaseSensitive(document_root, "image_height");
            assert(cJSON_IsNumber(image_height_document));
            image_height = image_height_document->valueint;

            log_info("Parsing metadata out of texture atlas: %s, width: %i, height: %i",
                     filename.c_str(), image_width, image_height);

            for (std::size_t index = 0; index < std::numeric_limits<std::size_t>::max(); index++)
            {
                TextureRegion region = TextureRegion::fromJson(index, document_root);
                if (!region.ok)
                {
                    break;
                }
                log_info("Parsed tile: %i", index);
                region.normalize(image_width, image_height);
                regions.push_back(region);
            }

            cJSON_Delete(document_root);
        }
        catch (const std::exception &e)
        {
            log_error("Exception while parsing spritesheet metadata JSON: %s", e.what());
            assert(false);
        }
        _texture_regions.emplace(in.type, regions);
    }
}
