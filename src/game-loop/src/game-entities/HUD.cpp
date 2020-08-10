#include <utility>
#include <sstream>
#include <string>

#include "game-entities/HUD.hpp"
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
    // Nothing to update.
}

void HUD::set_hearts_count(uint32_t hearts)
{
    _texts.hearts.set_text(to_string(hearts));
}

void HUD::set_ropes_count(uint32_t ropes)
{
    _texts.ropes.set_text(to_string(ropes));
}

void HUD::set_bombs_count(uint32_t bombs)
{
    _texts.bombs.set_text(to_string(bombs));
}

void HUD::set_dollars_count(uint32_t dollars)
{
    _texts.dollars.set_text(to_string(dollars));
}

HUD::HUD(std::shared_ptr<Viewport> viewport) : _viewport(std::move(viewport))
{
    _quads.heart.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::HEART);
    _quads.dollar.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::DOLLAR_SIGN);
    _quads.ropes.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::ROPE_ICON);
    _quads.bombs.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::BOMB_ICON);
    _quads.hold_item.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::HOLD_ITEM_ICON);

    const auto POS_X = static_cast<float>(_viewport->get_width_world_units() * 0.05f);
    const auto POS_Y = static_cast<float>(_viewport->get_height_world_units() * 0.05f);

    Point2D heart_center = {POS_X + (ICONS_OFFSET_WORLD_UNITS * 0), POS_Y};
    Point2D bombs_center = {POS_X + (ICONS_OFFSET_WORLD_UNITS * 1), POS_Y};
    Point2D ropes_center = {POS_X + (ICONS_OFFSET_WORLD_UNITS * 2), POS_Y};
    Point2D dollar_center = {POS_X + (ICONS_OFFSET_WORLD_UNITS * 3), POS_Y};
    Point2D hold_item_center = {POS_X + (ICONS_OFFSET_WORLD_UNITS * 4), POS_Y};

    _quads.heart.update(heart_center.x, heart_center.y);
    _quads.dollar.update(dollar_center.x, dollar_center.y);
    _quads.ropes.update(ropes_center.x, ropes_center.y);
    _quads.bombs.update(bombs_center.x, bombs_center.y);
    _quads.hold_item.update(hold_item_center.x, hold_item_center.y);

    _texts.hearts.set_position({heart_center.x + ICON_SIZE_WORLD_UNITS, heart_center.y});
    _texts.ropes.set_position({ropes_center.x + ICON_SIZE_WORLD_UNITS, ropes_center.y});
    _texts.bombs.set_position({bombs_center.x + ICON_SIZE_WORLD_UNITS, bombs_center.y});
    _texts.dollars.set_position({dollar_center.x + ICON_SIZE_WORLD_UNITS, dollar_center.y});
}
