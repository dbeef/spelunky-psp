#include <cstring>
#include <sstream>
#include <cassert>

#include "spritesheet-frames/HUDSpritesheetFrames.hpp"
#include "game-entities/PauseOverlay.hpp"
#include "Input.hpp"

namespace
{
    const char* PAUSED_MSG = "PAUSED";
}

void PauseOverlay::update(uint32_t delta_time_ms)
{
    if (_disabled_input)
    {
        return;
    }

    const auto& input = Input::instance();

    if (input.paused().changed() && input.paused().value())
    {
        if (!_paused)
        {
            // Just paused:
            _paused = true;

            _half_opaque_quad = std::make_shared<QuadComponent>(
                    TextureType::HUD,
                    Renderer::EntityType::SCREEN_SPACE,
                    _viewport->get_width_world_units(),
                    _viewport->get_height_world_units());

            _half_opaque_quad->frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::HALF_OPAQUE_TILE);
            _half_opaque_quad->update(_viewport->get_width_world_units() / 2.0f, _viewport->get_height_world_units() / 2.0f);

            {
                _texts.paused.set_scale(2.0f);

                const float text_width = std::strlen(PAUSED_MSG) * _texts.paused.get_font_width();
                const float text_center_x = (_viewport->get_width_world_units() / 2.0f) - (text_width / 2.0f) + (_texts.paused.get_font_width() / 2.0f);
                const float text_center_y = _viewport->get_height_world_units() * 0.8f;

                _texts.paused.set_position({text_center_x, text_center_y});
                _texts.paused.set_text(PAUSED_MSG);
            }

            {
                const std::string available_controls = get_available_controls_msg();

                const float text_width = available_controls.size() * _texts.controls.get_font_width();
                const float text_center_x = (_viewport->get_width_world_units() / 2.0f) - (text_width / 2.0f) + (_texts.controls.get_font_width() / 2.0f);
                const float text_center_y = _viewport->get_height_world_units() * 0.9f;

                _texts.controls.set_position({text_center_x, text_center_y});
                _texts.controls.set_text(available_controls);
            }
        }
        else
        {
            // Just unpaused:
            reset();
        }
    }

    if (_paused)
    {
        // Currently in pause:
        _quit_requested = input.quit_requested().value();
        _death_requested = input.death_requested().value();
    }
}

PauseOverlay::PauseOverlay(std::shared_ptr<Viewport> viewport, Type pause_screen_type)
    : _viewport(std::move(viewport))
    , _type(pause_screen_type)
{
}

PauseOverlay::~PauseOverlay()
{
    reset();
}

void PauseOverlay::reset()
{
    _paused = false;
    _death_requested = false;
    _quit_requested = false;

    _half_opaque_quad = nullptr;

    _texts.paused = {};
    _texts.controls = {};
}

std::string PauseOverlay::get_available_controls_msg() const
{
    std::stringstream out;

    switch(_type)
    {
        case Type::SCORES:
        case Type::MAIN_MENU:
        {
            out << Input::get_pause_binding_msg() << "-RETURN ";
            out << Input::get_quit_request_binding_msg() << "-QUIT";
            break;
        }
        case Type::PLAYING:
        {
            out << Input::get_pause_binding_msg() << "-RETURN ";
            out << Input::get_death_request_binding_msg() << "-DIE ";
            out << Input::get_quit_request_binding_msg() << "-QUIT";
            break;
        }
        default:
        {
            assert(false);
            return "";
        }
    }

    return out.str();
}
