#include <string>
#include <sstream>
#include <iomanip>

#include "game-entities/LevelSummaryOverlay.hpp"
#include "game-entities/LevelSummaryTracker.hpp"

namespace
{
    std::string get_level_completed_msg(uint32_t level_count)
    {
        std::stringstream out;
        out << "LEVEL" << " " << level_count << " COMPLETED!";
        return out.str();
    }

    std::string get_time_msg(uint32_t time_playing_current_level_ms, uint32_t time_playing_total_ms)
    {
        const auto time_playing_current_level_s = static_cast<uint32_t>(time_playing_current_level_ms / 1000) % 60;
        const auto time_playing_total_s = static_cast<uint32_t>(time_playing_total_ms / 1000) % 60;

        const auto time_playing_current_level_minutes = static_cast<uint32_t>(time_playing_current_level_ms / (60 * 1000));
        const auto time_playing_total_minutes = static_cast<uint32_t>(time_playing_total_ms / (60 * 1000));

        std::stringstream out;
        out << "TIME: " << std::setw(2) << std::left << std::setfill('0') << time_playing_current_level_minutes << ":"
                        << std::setw(2) << std::left << std::setfill('0') << time_playing_current_level_s << " / "
                        << std::setw(2) << std::left << std::setfill('0') << time_playing_total_minutes << ":"
                        << std::setw(2) << std::left << std::setfill('0') << time_playing_total_s;
        return out.str();
    }

    std::string get_money_msg(uint32_t money_beginning, uint32_t money_end)
    {
        std::stringstream out;
        out << "MONEY: " << "$" << money_beginning << " / " << "$" << money_end;
        return out.str();
    }

    const char* TIME_MSG = "TIME:";
    const char* LOOT_MSG = "LOOT:";
    const char* KILLS_MSG = "KILLS:";
    const char* MONEY_MSG = "MONEY:";
}

LevelSummaryOverlay::LevelSummaryOverlay(std::shared_ptr<Viewport> viewport, const std::shared_ptr<LevelSummaryTracker>& tracker)
    : _viewport(std::move(viewport))
{
    {
        const Point2D pos = {_viewport->get_width_world_units() * 0.16f, _viewport->get_height_world_units() * 0.155f};
        _texts.level_completed.set_yellow(true);
        _texts.level_completed.set_text(get_level_completed_msg(tracker->get_current_level()));
        _texts.level_completed.set_position(pos);
    }

    {
        const Point2D pos = {_viewport->get_width_world_units() * 0.16f, _viewport->get_height_world_units() * 0.23f};
        _texts.time.set_text(get_time_msg(tracker->get_time_playing_current_level_ms(), tracker->get_time_playing_total_ms()));
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
        _texts.money.set_text(get_money_msg(tracker->get_dollars_beginning(), tracker->get_dollars_end()));
        _texts.money.set_position(pos);
    }
}

void LevelSummaryOverlay::update(uint32_t delta_time_ms)
{
    // Nothing to update.
}
