#include "game-entities/SingleGoldBar.hpp"
#include "game-entities/TripleGoldBar.hpp"
#include "game-entities/BigGem.hpp"
#include "game-entities/LevelSummaryOverlay.hpp"
#include "game-entities/LevelSummaryTracker.hpp"

#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

namespace
{
    std::string get_level_completed_msg(uint32_t level_count)
    {
        std::stringstream out;
        out << "LEVEL " << level_count << " COMPLETED!";
        return out.str();
    }

    std::string get_time_msg(uint32_t time_playing_current_level_ms, uint32_t time_playing_total_ms)
    {
        const auto time_playing_current_level_s = static_cast<uint32_t>(time_playing_current_level_ms / 1000);
        const auto time_playing_current_level_minutes = static_cast<uint32_t>(time_playing_current_level_s / 60);

        const auto time_playing_total_s = static_cast<uint32_t>(time_playing_total_ms / 1000);
        const auto time_playing_total_minutes = static_cast<uint32_t>(time_playing_total_s / 60);

        std::stringstream out;
        out << "TIME: " << std::setw(2) << std::right << std::setfill('0') << time_playing_current_level_minutes << ":"
                        << std::setw(2) << std::right << std::setfill('0') << time_playing_current_level_s % 60 << " / "
                        << std::setw(2) << std::right << std::setfill('0') << time_playing_total_minutes << ":"
                        << std::setw(2) << std::right << std::setfill('0') << time_playing_total_s % 60;
        return out.str();
    }

    std::string get_money_msg(uint32_t money_beginning, uint32_t money_end)
    {
        std::stringstream out;
        out << "MONEY: " << "$" << money_beginning << " / " << "$" << money_end;
        return out.str();
    }

    const char* LOOT_MSG = "LOOT: ";
    const char* LOOT_NONE_MSG = "LOOT: NONE";

    const char* KILLS_MSG = "KILLS: ";
    const char* KILLS_NONE_MSG = "KILLS: NONE";
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
        _texts.loot.set_position(pos);

        if (tracker->get_loot_collected_events().empty())
        {
            _texts.loot.set_text(LOOT_NONE_MSG);
        }
        else
        {
            _texts.loot.set_text(LOOT_MSG);
        }

        float x_offset = 0;
        tracker->sort_loot_collected_events();

        for (const auto& event: tracker->get_loot_collected_events())
        {
            float x = 6.25f + x_offset;
            float y = 3.25f;

            switch (event)
            {
                // Spawn off-screen loot that will appear on the screen at intervals in the update method:
                case LootCollectedEvent::SINGLE_GOLD_BAR: _loot_entities.push_back(std::make_shared<SingleGoldBar>(x, y)); break;
                case LootCollectedEvent::TRIPLE_GOLD_BAR: _loot_entities.push_back(std::make_shared<TripleGoldBar>(x, y - 0.25f)); break;
                case LootCollectedEvent::GEM_DIAMOND:
                case LootCollectedEvent::GEM_EMERALD:
                case LootCollectedEvent::GEM_RUBY: _loot_entities.push_back(std::make_shared<BigGem>(x, y)); break;
                default: assert(false);
            }

            x_offset += 0.5f;
        }
    }

    {
        const Point2D pos = {_viewport->get_width_world_units() * 0.16f, _viewport->get_height_world_units() * 0.38f};
        _texts.kills.set_text(KILLS_NONE_MSG);
        _texts.kills.set_position(pos);
        // TODO: Render kills
    }

    {
        const Point2D pos = {_viewport->get_width_world_units() * 0.16f, _viewport->get_height_world_units() * 0.455f};
        _texts.money.set_text(get_money_msg(tracker->get_dollars_beginning(), tracker->get_dollars_end()));
        _texts.money.set_position(pos);
    }
}

void LevelSummaryOverlay::update(uint32_t delta_time_ms)
{
    // Make consecutive loot elements appear in an interval (calling update for the first time shows quad):

    _loot_appearing_timer += delta_time_ms;

    int elements_appearing = std::floor<int>(static_cast<float>(_loot_appearing_timer) / 250.0f);
    for (std::size_t index = 0; index < elements_appearing && index < _loot_entities.size(); index++)
    {
        auto& loot = _loot_entities[index];
        loot->update(0);
    }
}
