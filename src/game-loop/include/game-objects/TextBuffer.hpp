#pragma once

#include <cstdint>
#include <vector>

#include "spritesheet-frames/FontSpritesheetFrames.hpp"
#include "components/QuadComponent.hpp"
#include "GameObject.hpp"
#include "Point2D.hpp"

using TextEntityID = std::uint16_t;

class TextBuffer : public GameObject
{
public:

    static const TextEntityID INVALID_ENTITY = 0;

    void update(uint32_t delta_time_ms) override
    {
        for (const auto &id : _for_removal)
        {
            const auto predicate = [&id](const TextEntity &e) { return e.id == id; };
            const auto it = std::remove_if(_text_entries.begin(), _text_entries.end(), predicate);
            if (it != _text_entries.end())
            {
                _text_entries.erase(it);
            }
        }
    }

    TextEntityID create_text()
    {
        static TextEntityID unique_id_pool = std::numeric_limits<TextEntityID>::max();
        TextEntity e;
        e.id = --unique_id_pool;
        _text_entries.emplace_back(e);
        return unique_id_pool;
    }
    
    void remove_text(TextEntityID id)
    {
        _for_removal.push_back(id);
    }

    void update_text(TextEntityID id, Point2D position, const char* contents, std::size_t length)
    {
        auto it = std::find_if(_text_entries.begin(), _text_entries.end(), [id](const TextEntity& e) { return e.id == id; });
        if (it != _text_entries.end())
        {
            // Resize text if it won't fit full length of new contents:
            while (it->quads.size() < length)
            {
                it->quads.emplace_back(TextureType::FONT, Renderer::EntityType::SCREEN_SPACE, FONT_WIDTH_PIXELS, FONT_HEIGHT_PIXELS);
            }

            for (std::size_t index = 0; index < it->quads.size(); index++)
            {
                char sign = 0;

                if (index >= length)
                {
                    sign = ' ';
                }
                else
                {
                    sign = contents[index];
                }

                if (sign == 0)
                {
                    sign = ' ';
                }

                auto& quad = it->quads[index];
                const int16_t spritesheet_frame_index = sign + FONT_ASCII_OFFSET;

                // Update frame with ASCII sign:

                if (spritesheet_frame_index >= 0 &&
                    spritesheet_frame_index < static_cast<int16_t>(FontSpritesheetFrames::_SIZE))
                {
                    auto spritesheet_frame = static_cast<FontSpritesheetFrames>(spritesheet_frame_index);
                    quad.frame_changed(spritesheet_frame);
                }
                else
                {
                    assert(false);
                }

                // Update position:

                quad.update(position.x + (index * OFFSET_PIXELS), position.y);
            }
        }
        else
        {
            assert(false);
        }
    }

private:

    struct TextEntity
    {
        TextEntityID id{};
        std::vector<QuadComponent> quads;
    };

    std::vector<TextEntityID > _for_removal;
    std::vector<TextEntity> _text_entries;

    const float FONT_WIDTH_PIXELS = 16;
    const float FONT_HEIGHT_PIXELS = 16;
    const float OFFSET_PIXELS = 16;
};
