#include "TextureBank.hpp"
#include "graphics_utils/CreateTexture.hpp"
#include "logger/log.h"
#include "cJSON.h"
#include "assets/Assets.hpp"
#include "assets/Paths.hpp"

#include <cstdint>
#include <limits>
#include <vector>
#include <cassert>
#include <stdexcept>

REGISTER_SINGLETON_INSTANCE(TextureBank)

namespace
{
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
        {
            const auto [data, size] = Assets::instance().get(Paths::Spritesheets::LEVEL_TILES_PNG);
            out.push_back({ data, size, TextureType::CAVE_LEVEL_TILES} );
        }
        {
            const auto [data, size] = Assets::instance().get(Paths::Spritesheets::MAIN_DUDE_PNG);
            out.push_back({ data, size, TextureType::MAIN_DUDE} );
        }
        {
            const auto [data, size] = Assets::instance().get(Paths::Spritesheets::MAIN_MENU_PNG);
            out.push_back({ data, size, TextureType::MAIN_MENU} );
        }
        {
            const auto [data, size] = Assets::instance().get(Paths::Spritesheets::HUD_PNG);
            out.push_back({ data, size, TextureType::HUD} );
        }
        {
            const auto [data, size] = Assets::instance().get(Paths::Spritesheets::FONT_PNG);
            out.push_back({ data, size, TextureType::FONT} );
        }
        {
            const auto [data, size] = Assets::instance().get(Paths::Spritesheets::COLLECTIBLES_PNG);
            out.push_back({ data, size, TextureType::COLLECTIBLES} );
        }
        {
            const auto [data, size] = Assets::instance().get(Paths::Spritesheets::NPC_PNG);
            out.push_back({ data, size, TextureType::NPC} );
        }
        return out;
    }

    std::vector<TextureRegionInput> get_texture_region_input()
    {
        std::vector<TextureRegionInput> out;
        out.push_back({Assets::instance().get_ptr(Paths::Spritesheets::LEVEL_TILES_JSON), TextureType::CAVE_LEVEL_TILES});
        out.push_back({Assets::instance().get_ptr(Paths::Spritesheets::MAIN_DUDE_JSON), TextureType::MAIN_DUDE});
        out.push_back({Assets::instance().get_ptr(Paths::Spritesheets::MAIN_MENU_JSON), TextureType::MAIN_MENU});
        out.push_back({Assets::instance().get_ptr(Paths::Spritesheets::HUD_JSON), TextureType::HUD});
        out.push_back({Assets::instance().get_ptr(Paths::Spritesheets::FONT_JSON), TextureType::FONT});
        out.push_back({Assets::instance().get_ptr(Paths::Spritesheets::COLLECTIBLES_JSON), TextureType::COLLECTIBLES});
        out.push_back({Assets::instance().get_ptr(Paths::Spritesheets::NPC_JSON), TextureType::NPC});
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
                    break; // TODO some error indication?
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
