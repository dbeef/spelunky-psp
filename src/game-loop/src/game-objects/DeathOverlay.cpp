#include <utility>
#include <sstream>
#include <cstring>

#include "Input.hpp"
#include "game-objects/DeathOverlay.hpp"

namespace
{
    const char* GAME_OVER_MSG = "GAME OVER";
}

DeathOverlay::DeathOverlay(std::shared_ptr<Viewport>  viewport) : _viewport(std::move(viewport))
{
}

DeathOverlay::~DeathOverlay()
{
    _text_buffer->remove_text(_text_entity_ids.game_over);
    _text_buffer->remove_text(_text_entity_ids.controls);
}

void DeathOverlay::launch()
{
    _launched = true;

    _text_entity_ids.game_over = _text_buffer->create_text();
    _text_entity_ids.controls = _text_buffer->create_text();

    {
        const float text_width = std::strlen(GAME_OVER_MSG) * TextBuffer::get_font_width();
        const float text_center_x = (_viewport->get_width_world_units() / 2.0f) - (text_width / 2.0f) + (TextBuffer::get_font_width() / 2.0f);
        const float text_center_y = _viewport->get_height_world_units() * 0.25f;

        _text_buffer->update_text(_text_entity_ids.game_over, {text_center_x, text_center_y}, GAME_OVER_MSG, std::strlen(GAME_OVER_MSG));
    }

    {
        const std::string available_controls = get_available_controls_msg();
        const char* available_controls_cstr = available_controls.c_str();

        const float text_width = std::strlen(available_controls_cstr) * TextBuffer::get_font_width();
        const float text_center_x = (_viewport->get_width_world_units() / 2.0f) - (text_width / 2.0f) + (TextBuffer::get_font_width() / 2.0f);
        const float text_center_y = _viewport->get_height_world_units() * 0.45f;

        _text_buffer->update_text(_text_entity_ids.controls, {text_center_x, text_center_y}, available_controls_cstr, std::strlen(available_controls_cstr));
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

void DeathOverlay::set_text_buffer(const std::shared_ptr<TextBuffer> &text_buffer)
{
    assert(text_buffer != nullptr);
    _text_buffer = text_buffer;
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
