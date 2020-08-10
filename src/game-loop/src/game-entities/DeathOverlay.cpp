#include <utility>
#include <sstream>
#include <cstring>

#include "Input.hpp"
#include "game-entities/DeathOverlay.hpp"

namespace
{
    const char* GAME_OVER_MSG = "GAME OVER";
}

DeathOverlay::DeathOverlay(std::shared_ptr<Viewport>  viewport) : _viewport(std::move(viewport))
{
}

void DeathOverlay::launch()
{
    {
        _texts.game_over.set_scale(2.0f);

        const float text_width = std::strlen(GAME_OVER_MSG) * _texts.game_over.get_font_width();
        const float text_center_x = (_viewport->get_width_world_units() / 2.0f) - (text_width / 2.0f) + (_texts.game_over.get_font_width() / 2.0f);
        const float text_center_y = _viewport->get_height_world_units() * 0.25f;

        _texts.game_over.set_yellow(true);
        _texts.game_over.set_position({text_center_x, text_center_y});
        _texts.game_over.set_text(GAME_OVER_MSG);
    }

    {
        const std::string available_controls = get_available_controls_msg();

        const float text_width = available_controls.size() * _texts.controls.get_font_width();
        const float text_center_x = (_viewport->get_width_world_units() / 2.0f) - (text_width / 2.0f) + (_texts.controls.get_font_width() / 2.0f);
        const float text_center_y = _viewport->get_height_world_units() * 0.45f;

        _texts.controls.set_yellow(true);
        _texts.controls.set_position({text_center_x, text_center_y});
        _texts.controls.set_text(available_controls);
    }
}

void DeathOverlay::update(uint32_t delta_time_ms)
{
    if (_disabled_input)
    {
        return;
    }

    const auto& input = Input::instance();
    if (input.jumping().value())
    {
        _scores_requested = true;
    }
}

std::string DeathOverlay::get_available_controls_msg() const
{
    std::stringstream out;
    out << "PRESS ";
    out << Input::get_scores_request_binding_msg();
    out << " FOR HIGH SCORES.";
    return out.str();
}

void DeathOverlay::disable_input()
{
    _disabled_input = true;
}

void DeathOverlay::enable_input()
{
    _disabled_input = false;
}
