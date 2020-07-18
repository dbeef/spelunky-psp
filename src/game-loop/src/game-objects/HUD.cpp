#include <sstream>
#include <string>

#include "game-objects/HUD.hpp"
#include "spritesheet-frames/HUDSpritesheetFrames.hpp"

namespace
{
    // std::to_string is missing in PSP's libc++.
    std::string to_string(uint32_t value)
    {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }
}

void HUD::update(uint32_t delta_time_ms)
{
    _heart_quad.update(_heart_center.x, _heart_center.y);
    _dollar_quad.update(_dollar_center.x, _dollar_center.y);
    _ropes_quad.update(_ropes_center.x, _ropes_center.y);
    _bombs_quad.update(_bombs_center.x, _bombs_center.y);
    _hold_item_quad.update(_hold_item_center.x, _hold_item_center.y);
}

void HUD::set_text_buffer(const std::shared_ptr<TextBuffer> &text_buffer)
{
    assert(text_buffer != nullptr);
    _text_buffer = text_buffer;
    _text_entity_ids.hearts = text_buffer->create_text();
    _text_entity_ids.ropes = text_buffer->create_text();
    _text_entity_ids.bombs = text_buffer->create_text();
    _text_entity_ids.dollars = text_buffer->create_text();
}

void HUD::set_hearts_count(uint32_t hearts)
{
    const auto hearts_s = to_string(hearts);
    _text_buffer->update_text(_text_entity_ids.hearts, {_heart_center.x + TextBuffer::get_font_offset(), _heart_center.y},
                              hearts_s.c_str(), hearts_s.size());
}

void HUD::set_ropes_count(uint32_t ropes)
{
    const auto ropes_s = to_string(ropes);
    _text_buffer->update_text(_text_entity_ids.ropes, {_ropes_center.x + TextBuffer::get_font_offset(), _ropes_center.y},
                              ropes_s.c_str(), ropes_s.size());
}

void HUD::set_bombs_count(uint32_t bombs)
{
    const auto bombs_s = to_string(bombs);
    _text_buffer->update_text(_text_entity_ids.bombs, {_bombs_center.x + TextBuffer::get_font_offset(), _bombs_center.y},
                              bombs_s.c_str(), bombs_s.size());
}

void HUD::set_dollars_count(uint32_t dollars)
{
    const auto dollars_s = to_string(dollars);
    _text_buffer->update_text(_text_entity_ids.dollars, {_dollar_center.x + TextBuffer::get_font_offset(), _dollar_center.y},
                              dollars_s.c_str(), dollars_s.size());
}

HUD::HUD(std::shared_ptr<Viewport> viewport)
    : _heart_quad(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, TextBuffer::get_font_width(),
                  TextBuffer::get_font_height())
    , _dollar_quad(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, TextBuffer::get_font_width(),
                   TextBuffer::get_font_height())
    , _ropes_quad(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, TextBuffer::get_font_width(),
                  TextBuffer::get_font_height())
    , _bombs_quad(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, TextBuffer::get_font_width(),
                  TextBuffer::get_font_height())
    , _hold_item_quad(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, TextBuffer::get_font_width(),
                      TextBuffer::get_font_height())
    , _text_buffer(nullptr)
    , _viewport(std::move(viewport))
{
    _heart_quad.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::HEART);
    _dollar_quad.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::DOLLAR_SIGN);
    _ropes_quad.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::ROPE_ICON);
    _bombs_quad.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::BOMB_ICON);
    _hold_item_quad.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::HOLD_ITEM_ICON);

    const float ICONS_OFFSET_WORLD_UNITS = TextBuffer::get_font_width() * 3.0f;
    
    const auto POS_X = static_cast<float>(_viewport->get_width_world_units() * 0.05f);
    const auto POS_Y = static_cast<float>(_viewport->get_height_world_units() * 0.05f);

    _heart_center.x = POS_X + (ICONS_OFFSET_WORLD_UNITS * 0);
    _bombs_center.x = POS_X + (ICONS_OFFSET_WORLD_UNITS * 1);
    _ropes_center.x = POS_X + (ICONS_OFFSET_WORLD_UNITS * 2);
    _dollar_center.x = POS_X + (ICONS_OFFSET_WORLD_UNITS * 3);
    _hold_item_center.x = POS_X + (ICONS_OFFSET_WORLD_UNITS * 4);

    _heart_center.y = POS_Y;
    _dollar_center.y = POS_Y;
    _ropes_center.y = POS_Y;
    _bombs_center.y = POS_Y;
    _hold_item_center.y = POS_Y;
}

HUD::~HUD()
{
    _text_buffer->remove_text(_text_entity_ids.dollars);
    _text_buffer->remove_text(_text_entity_ids.hearts);
    _text_buffer->remove_text(_text_entity_ids.bombs);
    _text_buffer->remove_text(_text_entity_ids.ropes);
}
