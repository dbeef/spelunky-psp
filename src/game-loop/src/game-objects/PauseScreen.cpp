#include <cstring>
#include <sstream>
#include <cassert>

#include "spritesheet-frames/HUDSpritesheetFrames.hpp"
#include "game-objects/PauseScreen.hpp"
#include "Input.hpp"

namespace
{
    const char* PAUSED_MSG = "PAUSED";
}

void PauseScreen::set_text_buffer(const std::shared_ptr<TextBuffer> &text_buffer)
{
    assert(text_buffer != nullptr);
    _text_buffer = text_buffer;
}

void PauseScreen::update(uint32_t delta_time_ms)
{
    const auto& input = Input::instance();

    if (_last_pause_check != input.paused())
    {
        _last_pause_check = input.paused();

        if (!_paused && _last_pause_check)
        {
            // Just paused
            _paused = true;

            _half_opaque_quad = std::make_shared<QuadComponent>(
                    TextureType::HUD,
                    Renderer::EntityType::SCREEN_SPACE,
                    _viewport->get_width_world_units(),
                    _viewport->get_height_world_units());

            _half_opaque_quad->frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::HALF_OPAQUE_TILE);
            _half_opaque_quad->update(_viewport->get_width_world_units() / 2.0f, _viewport->get_height_world_units() / 2.0f);

            _text_entity_ids.paused = _text_buffer->create_text();
            _text_entity_ids.controls = _text_buffer->create_text();

            {
                const float text_width = std::strlen(PAUSED_MSG) * TextBuffer::get_font_width();
                const float text_center_x = (_viewport->get_width_world_units() / 2.0f) - (text_width / 2.0f) + (TextBuffer::get_font_width() / 2.0f);
                const float text_center_y = _viewport->get_height_world_units() * 0.8f;

                _text_buffer->update_text(_text_entity_ids.paused, {text_center_x, text_center_y}, PAUSED_MSG, std::strlen(PAUSED_MSG));
            }

            {
                const std::string available_controls = get_available_controls_msg();
                const char* available_controls_cstr = available_controls.c_str();

                const float text_width = std::strlen(available_controls_cstr) * TextBuffer::get_font_width();
                const float text_center_x = (_viewport->get_width_world_units() / 2.0f) - (text_width / 2.0f) + (TextBuffer::get_font_width() / 2.0f);
                const float text_center_y = _viewport->get_height_world_units() * 0.9f;

                _text_buffer->update_text(_text_entity_ids.controls, {text_center_x, text_center_y}, available_controls_cstr, std::strlen(available_controls_cstr));
            }
        }
        else if (_paused && _last_pause_check)
        {
            // Just unpaused
            _paused = false;
            _half_opaque_quad = nullptr;
            _text_buffer->remove_text(_text_entity_ids.paused);
            _text_buffer->remove_text(_text_entity_ids.controls);
            _text_entity_ids.paused = TextBuffer::INVALID_ENTITY;
            _text_entity_ids.controls = TextBuffer::INVALID_ENTITY;
        }
    }
    else if (_paused)
    {
        // Currently in pause
        _quit_requested = input.quit_requested();
        _death_requested = input.death_requested();
    }
}

PauseScreen::PauseScreen(std::shared_ptr<Viewport> viewport, Type pause_screen_type)
    : _viewport(std::move(viewport))
    , _type(pause_screen_type)
{
}

PauseScreen::~PauseScreen()
{
    _text_buffer->remove_text(_text_entity_ids.paused);
    _text_buffer->remove_text(_text_entity_ids.controls);
}

void PauseScreen::unpause()
{
    _paused = false;
    _last_pause_check = false;
}

std::string PauseScreen::get_available_controls_msg() const
{
    std::stringstream out;

    switch(_type)
    {
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
