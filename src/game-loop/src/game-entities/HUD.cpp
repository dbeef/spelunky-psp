#include <utility>
#include <sstream>
#include <string>

#include "game-entities/HUD.hpp"
#include "spritesheet-frames/HUDSpritesheetFrames.hpp"
#include "other/Inventory.hpp"

namespace
{
    const float ICONS_OFFSET_WORLD_UNITS = 1.5f;
    const float ICON_SIZE_WORLD_UNITS = 0.5f;

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
    _dollars_buffer_count_timer += delta_time_ms;

    // TODO: User defined literal for milliseconds.
    if (_dollars_buffer_count_timer > 90)
    {
        _dollars_buffer_count_timer = 0;

        if (_dollars_buffer_count > 0)
        {
            const int diff = std::min<int>(100, _dollars_buffer_count);

            _dollars_count += diff;
            _dollars_buffer_count -= diff;

            update_dollars();
        }
    }
}

HUD::HUD(std::shared_ptr<Viewport> viewport) : _viewport(std::move(viewport))
{
    assert(_viewport);

    _quads.heart = std::make_shared<QuadComponent>(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, ICON_SIZE_WORLD_UNITS, ICON_SIZE_WORLD_UNITS);
    _quads.dollar = std::make_shared<QuadComponent>(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, ICON_SIZE_WORLD_UNITS, ICON_SIZE_WORLD_UNITS);
    _quads.ropes = std::make_shared<QuadComponent>(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, ICON_SIZE_WORLD_UNITS, ICON_SIZE_WORLD_UNITS);
    _quads.bombs = std::make_shared<QuadComponent>(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, ICON_SIZE_WORLD_UNITS, ICON_SIZE_WORLD_UNITS);
    _quads.hold_item = std::make_shared<QuadComponent>(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, ICON_SIZE_WORLD_UNITS, ICON_SIZE_WORLD_UNITS);

    _quads.heart->frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::HEART);
    _quads.dollar->frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::DOLLAR_SIGN);
    _quads.ropes->frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::ROPE_ICON);
    _quads.bombs->frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::BOMB_ICON);
    _quads.hold_item->frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::HOLD_ITEM_ICON);

    const auto POS_X = static_cast<float>(_viewport->get_width_world_units() * 0.05f);
    const auto POS_Y = static_cast<float>(_viewport->get_height_world_units() * 0.05f);

    Point2D heart_center = {POS_X + (ICONS_OFFSET_WORLD_UNITS * 0), POS_Y};
    Point2D bombs_center = {POS_X + (ICONS_OFFSET_WORLD_UNITS * 1), POS_Y};
    Point2D ropes_center = {POS_X + (ICONS_OFFSET_WORLD_UNITS * 2), POS_Y};
    Point2D dollar_center = {POS_X + (ICONS_OFFSET_WORLD_UNITS * 3), POS_Y};
    Point2D hold_item_center = {POS_X + (ICONS_OFFSET_WORLD_UNITS * (0)), POS_Y + (ICONS_OFFSET_WORLD_UNITS * 0.5f)};

    _quads.heart->update(heart_center.x, heart_center.y);
    _quads.dollar->update(dollar_center.x, dollar_center.y);
    _quads.ropes->update(ropes_center.x, ropes_center.y);
    _quads.bombs->update(bombs_center.x, bombs_center.y);
    _quads.hold_item->update(hold_item_center.x, hold_item_center.y);

    _texts.hearts.set_position({heart_center.x + ICON_SIZE_WORLD_UNITS, heart_center.y});
    _texts.ropes.set_position({ropes_center.x + ICON_SIZE_WORLD_UNITS, ropes_center.y});
    _texts.bombs.set_position({bombs_center.x + ICON_SIZE_WORLD_UNITS, bombs_center.y});
    _texts.dollars.set_position({dollar_center.x + ICON_SIZE_WORLD_UNITS, dollar_center.y});

    _texts.hearts.set_text(to_string(Inventory::instance().get_hearts()));
    _texts.ropes.set_text(to_string(Inventory::instance().get_ropes()));
    _texts.bombs.set_text(to_string(Inventory::instance().get_bombs()));
    _texts.dollars.set_text(to_string(Inventory::instance().get_dollars()));

    _texts.dollars_buffer.set_scale(0.5f);
    _texts.dollars_buffer.set_yellow(true);

    _dollars_count_previously = Inventory::instance().get_dollars();
    _dollars_count = Inventory::instance().get_dollars();

    Inventory::instance().add_observer(this);
}

void HUD::on_notify(const InventoryEvent * event)
{
    switch(*event)
    {
        case InventoryEvent::HEARTS_COUNT_CHANGED: _texts.hearts.set_text(to_string(Inventory::instance().get_hearts())); break;
        case InventoryEvent::DOLLARS_COUNT_CHANGED:
        {
            _dollars_buffer_count += Inventory::instance().get_dollars() - _dollars_count_previously;
            _dollars_count_previously = Inventory::instance().get_dollars();

            update_dollars();
            break;
        }
        default: break;
    }
}

HUD::~HUD()
{
    Inventory::instance().remove_observer(this);
}

void HUD::hide()
{
    _dollars_buffer_count = 0;
    _dollars_count = 0;
    _dollars_count_previously = 0;

    _quads.heart = nullptr;
    _quads.dollar = nullptr;
    _quads.ropes = nullptr;
    _quads.bombs = nullptr;
    _quads.hold_item = nullptr;

    _texts.dollars = {};
    _texts.dollars_buffer = {};
    _texts.hearts = {};
    _texts.ropes = {};
    _texts.bombs = {};
}

void HUD::update_dollars()
{
    Point2D buff_position = _texts.dollars.get_pos_screen_space();
    buff_position.x += _texts.dollars.get_width();

    _texts.dollars_buffer.set_position(buff_position);
    _texts.dollars_buffer.set_text("+" + to_string(_dollars_buffer_count));

    _texts.dollars.set_text(to_string(_dollars_count));

    if (_dollars_buffer_count == 0)
    {
        _texts.dollars_buffer.set_text("");
    }
}
