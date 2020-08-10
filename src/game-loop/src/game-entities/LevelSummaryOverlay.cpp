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
    {
        const Point2D pos = {_viewport->get_width_world_units() * 0.16f, _viewport->get_height_world_units() * 0.155f};
        _texts.level_completed.set_yellow(true);
        _texts.level_completed.set_text(LEVEL_COMPLETED_MSG);
        _texts.level_completed.set_position(pos);
    }

    {
        const Point2D pos = {_viewport->get_width_world_units() * 0.16f, _viewport->get_height_world_units() * 0.23f};
        _texts.time.set_text(TIME_MSG);
        _texts.time.set_position(pos);
    }

    {
        const Point2D pos = {_viewport->get_width_world_units() * 0.16f, _viewport->get_height_world_units() * 0.305f};
        _texts.loot.set_text(LOOT_MSG);
        _texts.loot.set_position(pos);
    }

    {
        const Point2D pos = {_viewport->get_width_world_units() * 0.16f, _viewport->get_height_world_units() * 0.38f};
        _texts.kills.set_text(KILLS_MSG);
        _texts.kills.set_position(pos);
    }

    {
        const Point2D pos = {_viewport->get_width_world_units() * 0.16f, _viewport->get_height_world_units() * 0.455f};
        _texts.money.set_text(MONEY_MSG);
        _texts.money.set_position(pos);
    }
}

void LevelSummaryOverlay::update(uint32_t delta_time_ms)
{
    // Nothing to update.
}
