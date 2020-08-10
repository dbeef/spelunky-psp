#include <cstring>
#include <sstream>
#include <iomanip>

#include "game-entities/ScoresOverlay.hpp"

namespace
{
    const char* TOP_DEFILERS_MSG = "TOP DEFILERS";
    const char* STATISTICS_MSG = "STATISTICS";
    const char* MONEY_MSG = "MONEY:";
    const char* KILLS_MSG = "KILLS:";
    const char* SAVES_MSG = "SAVES:";
    const char* PLAYS_MSG = "PLAYS:";
    const char* DEATHS_MSG = "DEATHS:";
    const char* WINS_MSG = "WINS:";

    // TODO: Pass actual numbers.
    const int MONEY = 0;
    const int KILLS = 0;
    const int SAVES = 0;
    const int PLAYS = 0;
    const int DEATHS = 0;
    const int WINS = 0;

    std::string format(const char* msg, int value)
    {
        std::stringstream out;
        out << std::setw(8) << std::left;
        out << msg << " " << value;
        return out.str();
    }
}

ScoresOverlay::ScoresOverlay(std::shared_ptr<Viewport> viewport) : _viewport(std::move(viewport))
{
    {
        const float text_width = std::strlen(TOP_DEFILERS_MSG) * _texts.top_defilers.get_font_width();
        const Point2D position = {
                (_viewport->get_width_world_units() * 0.625f) - (text_width / 2.0f) + (_texts.top_defilers.get_font_width() / 2.0f),
                _viewport->get_height_world_units() * 0.15f
        };

        _texts.top_defilers.set_position(position);
        _texts.top_defilers.set_text(TOP_DEFILERS_MSG);
        _texts.top_defilers.set_yellow(true);
    }

    {
        const Point2D position = {_viewport->get_width_world_units() * 0.4f, _viewport->get_height_world_units() * 0.23f};
        _texts.money.set_position(position);
        _texts.money.set_text(format(MONEY_MSG, MONEY));
    }

    {
        const Point2D position = {_viewport->get_width_world_units() * 0.4f, _viewport->get_height_world_units() * 0.31f};
        _texts.kills.set_position(position);
        _texts.kills.set_text(format(KILLS_MSG, KILLS));
    }

    {
        const Point2D position = {_viewport->get_width_world_units() * 0.4f, _viewport->get_height_world_units() * 0.39f};
        _texts.saves.set_position(position);
        _texts.saves.set_text(format(SAVES_MSG, SAVES));
    }

    {
        const float text_width = std::strlen(STATISTICS_MSG) * _texts.statistics.get_font_width();
        const Point2D position = {
                (_viewport->get_width_world_units() * 0.625f) - (text_width / 2.0f) + (_texts.statistics.get_font_width() / 2.0f),
                _viewport->get_height_world_units() * 0.47f
        };

        _texts.statistics.set_position(position);
        _texts.statistics.set_text(STATISTICS_MSG);
        _texts.statistics.set_yellow(true);
    }

    {
        const Point2D position = {_viewport->get_width_world_units() * 0.4f, _viewport->get_height_world_units() * 0.55f};
        _texts.plays.set_position(position);
        _texts.plays.set_text(format(PLAYS_MSG, PLAYS));
    }

    {
        const Point2D position = {_viewport->get_width_world_units() * 0.4f, _viewport->get_height_world_units() * 0.63f};
        _texts.deaths.set_position(position);
        _texts.deaths.set_text(format(DEATHS_MSG, DEATHS));
    }

    {
        const Point2D position = {_viewport->get_width_world_units() * 0.4f, _viewport->get_height_world_units() * 0.71f};
        _texts.wins.set_position(position);
        _texts.wins.set_text(format(WINS_MSG, WINS));
    }
}

void ScoresOverlay::update(uint32_t delta_time_ms)
{
    // Nothing to update.
}
