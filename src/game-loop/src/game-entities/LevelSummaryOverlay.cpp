#include <cstring>
#include <game-entities/LevelSummaryOverlay.hpp>
#include "game-entities/LevelSummaryOverlay.hpp"

namespace
{
    const char* LEVEL_COMPLETED_MSG = "LEVEL COMPLETED!";
    const char* TIME_MSG = "TIME:";
    const char* LOOT_MSG = "LOOT:";
    const char* KILLS_MSG = "KILLS:";
    const char* MONEY_MSG = "MONEY:";
}

LevelSummaryOverlay::LevelSummaryOverlay(std::shared_ptr<Viewport> viewport) : _viewport(std::move(viewport))
{
}

LevelSummaryOverlay::~LevelSummaryOverlay()
{
    _text_buffer->remove_text(_text_entity_ids.level_completed);
    _text_buffer->remove_text(_text_entity_ids.time);
    _text_buffer->remove_text(_text_entity_ids.loot);
    _text_buffer->remove_text(_text_entity_ids.kills);
    _text_buffer->remove_text(_text_entity_ids.money);
}

void LevelSummaryOverlay::launch()
{
    if (_launched)
    {
        return;
    }

    _text_entity_ids.level_completed = _text_buffer->create_text();
    _text_entity_ids.time = _text_buffer->create_text();
    _text_entity_ids.loot = _text_buffer->create_text();
    _text_entity_ids.kills = _text_buffer->create_text();
    _text_entity_ids.money = _text_buffer->create_text();

    {
        const Point2D pos = {_viewport->get_width_world_units() * 0.16f, _viewport->get_height_world_units() * 0.155f};
        _text_buffer->update_text(_text_entity_ids.level_completed, pos, LEVEL_COMPLETED_MSG, std::strlen(LEVEL_COMPLETED_MSG), true);
    }

    {
        const Point2D pos = {_viewport->get_width_world_units() * 0.16f, _viewport->get_height_world_units() * 0.23f};
        _text_buffer->update_text(_text_entity_ids.time, pos, TIME_MSG, std::strlen(TIME_MSG));
    }

    {
        const Point2D pos = {_viewport->get_width_world_units() * 0.16f, _viewport->get_height_world_units() * 0.305f};
        _text_buffer->update_text(_text_entity_ids.loot, pos, LOOT_MSG, std::strlen(LOOT_MSG));
    }

    {
        const Point2D pos = {_viewport->get_width_world_units() * 0.16f, _viewport->get_height_world_units() * 0.38f};
        _text_buffer->update_text(_text_entity_ids.kills, pos, KILLS_MSG, std::strlen(KILLS_MSG));
    }

    {
        const Point2D pos = {_viewport->get_width_world_units() * 0.16f, _viewport->get_height_world_units() * 0.455f};
        _text_buffer->update_text(_text_entity_ids.money, pos, MONEY_MSG, std::strlen(MONEY_MSG));
    }

    _launched = true;
}

void LevelSummaryOverlay::update(uint32_t delta_time_ms)
{
    // Nothing to update.
}

void LevelSummaryOverlay::set_text_buffer(const std::shared_ptr<TextBuffer> &text_buffer)
{
    assert(text_buffer != nullptr);
    _text_buffer = text_buffer;
}