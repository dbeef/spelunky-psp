#include "spritesheet-frames/FontSpritesheetFrames.hpp"
#include "components/FontComponent.hpp"

void FontComponent::update()
{
    // Resize text if it won't fit full length of new contents:
    while (_quads.size() < _properties.text.size())
    {
        _quads.emplace_back(TextureType::FONT, Renderer::EntityType::SCREEN_SPACE, get_font_width(), get_font_height());
    }

    for (std::size_t index = 0; index < _quads.size(); index++)
    {
        char sign = 0;

        if (index >= _properties.text.size())
        {
            sign = ' ';
        } else
        {
            sign = _properties.text[index];
        }

        if (sign == 0)
        {
            sign = ' ';
        }

        auto &quad = _quads[index];
        const int16_t spritesheet_frame_index = sign + (_properties.yellow ? FONT_ASCII_YELLOW_OFFSET : FONT_ASCII_OFFSET);

        // Update frame with ASCII sign:

        if (spritesheet_frame_index >= 0 &&
            spritesheet_frame_index < static_cast<int16_t>(FontSpritesheetFrames::_SIZE))
        {
            auto spritesheet_frame = static_cast<FontSpritesheetFrames>(spritesheet_frame_index);
            quad.frame_changed(spritesheet_frame);
        } else
        {
            assert(false);
        }

        // Update position:

        quad.update(_properties.pos_screen_space.x + (index * get_font_width()), _properties.pos_screen_space.y);
    }
}
