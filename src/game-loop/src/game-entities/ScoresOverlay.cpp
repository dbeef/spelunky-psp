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
}

ScoresOverlay::~ScoresOverlay()
{
    _text_buffer->remove_text(_text_entity_ids.top_defilers);
    _text_buffer->remove_text(_text_entity_ids.statistics);
    _text_buffer->remove_text(_text_entity_ids.money);
    _text_buffer->remove_text(_text_entity_ids.kills);
    _text_buffer->remove_text(_text_entity_ids.saves);
    _text_buffer->remove_text(_text_entity_ids.plays);
    _text_buffer->remove_text(_text_entity_ids.deaths);
    _text_buffer->remove_text(_text_entity_ids.wins);
}

void ScoresOverlay::launch()
{
    if (_launched)
    {
        return;
    }

    _text_entity_ids.top_defilers = _text_buffer->create_text();
    _text_entity_ids.statistics = _text_buffer->create_text();
    _text_entity_ids.money = _text_buffer->create_text();
    _text_entity_ids.kills = _text_buffer->create_text();
    _text_entity_ids.saves = _text_buffer->create_text();
    _text_entity_ids.plays = _text_buffer->create_text();
    _text_entity_ids.deaths = _text_buffer->create_text();
    _text_entity_ids.wins = _text_buffer->create_text();

    {
        const float text_width = std::strlen(TOP_DEFILERS_MSG) * TextBuffer::get_font_width();
        const Point2D pos = {
                (_viewport->get_width_world_units() * 0.625f) - (text_width / 2.0f) + (TextBuffer::get_font_width() / 2.0f),
                _viewport->get_height_world_units() * 0.15f
        };
        _text_buffer->update_text(_text_entity_ids.top_defilers, pos, TOP_DEFILERS_MSG, std::strlen(TOP_DEFILERS_MSG), 1.0f, true);
    }

    {
        const auto msg = format(MONEY_MSG, MONEY);
        const Point2D pos = {_viewport->get_width_world_units() * 0.4f, _viewport->get_height_world_units() * 0.23f};
        _text_buffer->update_text(_text_entity_ids.money, pos, msg.c_str(), std::strlen(msg.c_str()));
    }

    {
        const auto msg = format(KILLS_MSG, KILLS);
        const Point2D pos = {_viewport->get_width_world_units() * 0.4f, _viewport->get_height_world_units() * 0.31f};
        _text_buffer->update_text(_text_entity_ids.kills, pos, msg.c_str(), std::strlen(msg.c_str()));
    }

    {
        const auto msg = format(SAVES_MSG, SAVES);
        const Point2D pos = {_viewport->get_width_world_units() * 0.4f, _viewport->get_height_world_units() * 0.39f};
        _text_buffer->update_text(_text_entity_ids.saves, pos, msg.c_str(), std::strlen(msg.c_str()));
    }

    {
        const float text_width = std::strlen(STATISTICS_MSG) * TextBuffer::get_font_width();
        const Point2D pos = {
                (_viewport->get_width_world_units() * 0.625f) - (text_width / 2.0f) + (TextBuffer::get_font_width() / 2.0f),
                _viewport->get_height_world_units() * 0.47f
        };
        _text_buffer->update_text(_text_entity_ids.statistics, pos, STATISTICS_MSG, std::strlen(STATISTICS_MSG), 1.0f, true);
    }

    {
        const auto msg = format(PLAYS_MSG, PLAYS);
        const Point2D pos = {_viewport->get_width_world_units() * 0.4f, _viewport->get_height_world_units() * 0.55f};
        _text_buffer->update_text(_text_entity_ids.plays, pos, msg.c_str(), std::strlen(msg.c_str()));
    }

    {
        const auto msg = format(DEATHS_MSG, DEATHS);
        const Point2D pos = {_viewport->get_width_world_units() * 0.4f, _viewport->get_height_world_units() * 0.63f};
        _text_buffer->update_text(_text_entity_ids.deaths, pos, msg.c_str(), std::strlen(msg.c_str()));
    }

    {
        const auto msg = format(WINS_MSG, WINS);
        const Point2D pos = {_viewport->get_width_world_units() * 0.4f, _viewport->get_height_world_units() * 0.71f};
        _text_buffer->update_text(_text_entity_ids.wins, pos, msg.c_str(), std::strlen(msg.c_str()));
    }

    _launched = true;
}

void ScoresOverlay::update(uint32_t delta_time_ms)
{
    // Nothing to update.
}

void ScoresOverlay::set_text_buffer(const std::shared_ptr<TextBuffer> &text_buffer)
{
    assert(text_buffer != nullptr);
    _text_buffer = text_buffer;
}
