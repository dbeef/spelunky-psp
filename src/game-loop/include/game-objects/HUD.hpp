#pragma once

#include <memory>
#include <sstream>

#include "components/QuadComponent.hpp"
#include "GameObject.hpp"
#include "spritesheet-frames/HUDSpritesheetFrames.hpp"
#include "Point2D.hpp"
#include "TextBuffer.hpp"

class HUD : public GameObject
{
public:

    HUD(float pos_x, float pos_y)
            : _heart_quad(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, ICON_WIDTH_PIXELS, ICON_HEIGHT_PIXELS)
            , _dollar_quad(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, ICON_WIDTH_PIXELS, ICON_HEIGHT_PIXELS)
            , _ropes_quad(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, ICON_WIDTH_PIXELS, ICON_HEIGHT_PIXELS)
            , _bombs_quad(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, ICON_WIDTH_PIXELS, ICON_HEIGHT_PIXELS)
            , _hold_item_quad(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, ICON_WIDTH_PIXELS, ICON_HEIGHT_PIXELS)
            , _text_buffer(nullptr)
    {
        _heart_quad.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::HEART);
        _dollar_quad.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::DOLLAR_SIGN);
        _ropes_quad.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::ROPE_ICON);
        _bombs_quad.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::BOMB_ICON);
        _hold_item_quad.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::HOLD_ITEM_ICON);

        icons_offset_pixels = Video::instance().get_window_width() * 0.1f;

        _heart_center.x = pos_x + (icons_offset_pixels * 0);
        _bombs_center.x = pos_x + (icons_offset_pixels * 1);
        _ropes_center.x = pos_x + (icons_offset_pixels * 2);
        _dollar_center.x = pos_x + (icons_offset_pixels * 3);
        _hold_item_center.x = pos_x + (icons_offset_pixels * 4);

        _heart_center.y = pos_y;
        _dollar_center.y = pos_y;
        _ropes_center.y = pos_y;
        _bombs_center.y = pos_y;
        _hold_item_center.y = pos_y;
    }

    void update(uint32_t delta_time_ms) override
    {
        _heart_quad.update(_heart_center.x, _heart_center.y);
        _dollar_quad.update(_dollar_center.x, _dollar_center.y);
        _ropes_quad.update(_ropes_center.x, _ropes_center.y);
        _bombs_quad.update(_bombs_center.x, _bombs_center.y);
        _hold_item_quad.update(_bombs_center.x, _bombs_center.y);
    }

    void set_text_buffer(const std::shared_ptr<TextBuffer>& text_buffer)
    {
        assert(text_buffer != nullptr);
        _text_buffer = text_buffer;
        _text_entity_ids.hearts = text_buffer->create_text();
        _text_entity_ids.ropes = text_buffer->create_text();
        _text_entity_ids.bombs = text_buffer->create_text();
        _text_entity_ids.dollars = text_buffer->create_text();
    };

    void set_hearts_count(uint32_t hearts)
    {
        // std::to_string missing in PSP's libc++.

        std::stringstream ss;
        ss << hearts;

        _text_buffer->update_text(_text_entity_ids.hearts, {_heart_center.x + (icons_offset_pixels / 2.0f), _heart_center.y},
                                  ss.str().c_str(), ss.str().size());
    }

    void set_ropes_count(uint32_t ropes)
    {
        // std::to_string missing in PSP's libc++.

        std::stringstream ss;
        ss << ropes;

        _text_buffer->update_text(_text_entity_ids.ropes, {_ropes_center.x + (icons_offset_pixels / 2.0f), _ropes_center.y},
                                  ss.str().c_str(), ss.str().size());
    }

    void set_bombs_count(uint32_t bombs)
    {
        // std::to_string missing in PSP's libc++.

        std::stringstream ss;
        ss << bombs;

        _text_buffer->update_text(_text_entity_ids.bombs, {_bombs_center.x + (icons_offset_pixels / 2.0f), _bombs_center.y},
                                  ss.str().c_str(), ss.str().size());
    }

    void set_dollars_count(uint32_t dollars)
    {
        // std::to_string missing in PSP's libc++.

        std::stringstream ss;
        ss << dollars;

        _text_buffer->update_text(_text_entity_ids.dollars, {_dollar_center.x + (icons_offset_pixels / 2.0f), _dollar_center.y},
                                  ss.str().c_str(), ss.str().size());
    }

private:

    const float ICON_WIDTH_PIXELS = 16;
    const float ICON_HEIGHT_PIXELS = 16;
    float icons_offset_pixels = 0.0f;

    Point2D _heart_center;
    Point2D _dollar_center;
    Point2D _ropes_center;
    Point2D _bombs_center;
    Point2D _hold_item_center;

    QuadComponent _heart_quad;
    QuadComponent _dollar_quad;
    QuadComponent _ropes_quad;
    QuadComponent _bombs_quad;
    QuadComponent _hold_item_quad;

    std::shared_ptr<TextBuffer> _text_buffer;
    struct
    {
        TextEntityID hearts = TextBuffer::INVALID_ENTITY;
        TextEntityID ropes = TextBuffer::INVALID_ENTITY;
        TextEntityID bombs = TextBuffer::INVALID_ENTITY;
        TextEntityID dollars = TextBuffer::INVALID_ENTITY;
    } _text_entity_ids;
};
