#pragma once

#include <cstdint>
#include <vector>

#include "viewport/Viewport.hpp"
#include "spritesheet-frames/FontSpritesheetFrames.hpp"
#include "components/QuadComponent.hpp"
#include "GameEntity.hpp"
#include "Point2D.hpp"

using TextEntityID = std::uint16_t;

class TextBuffer : public GameEntity
{
public:

    static const TextEntityID INVALID_ENTITY = 0;
    static WorldUnit_t get_font_width() { return 0.5f; }
    static WorldUnit_t get_font_height() { return 0.5f; }
    static WorldUnit_t get_font_offset() { return 0.5f; }

    void update(uint32_t delta_time_ms) override;
    TextEntityID create_text();
    void remove_text(TextEntityID id) { _for_removal.push_back(id); }
    void update_text(TextEntityID id, Point2D position, const char* contents, std::size_t length, bool yellow=false);

private:

    struct TextEntity
    {
        TextEntityID id{};
        std::vector<QuadComponent> quads;
    };

    std::vector<TextEntityID > _for_removal;
    std::vector<TextEntity> _text_entries;
};
