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

    void update(uint32_t delta_time_ms) override;
    TextEntityID create_text();
    void remove_text(TextEntityID id) { _for_removal.push_back(id); }
    void update_text(TextEntityID id, Point2D position, const char* contents, std::size_t length);

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
