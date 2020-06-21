#include <cstring>

#include "spritesheet-frames/HUDSpritesheetFrames.hpp"
#include "game-objects/PausePlayingGame.hpp"
#include "Input.hpp"

namespace
{
    const char* PAUSED_MSG = "PAUSED";
}

void PausePlayingGame::set_text_buffer(const std::shared_ptr<TextBuffer> &text_buffer)
{
    assert(text_buffer != nullptr);
    _text_buffer = text_buffer;
}

void PausePlayingGame::update(uint32_t delta_time_ms)
{
    const auto& input = Input::instance();

    if (_last_pause_check != input.paused())
    {
        _last_pause_check = input.paused();

        if (!_paused && _last_pause_check)
        {
            _paused = true;

            _half_opaque_quad = std::make_shared<QuadComponent>(
                    TextureType::HUD,
                    Renderer::EntityType::SCREEN_SPACE,
                    _viewport->get_width(),
                    _viewport->get_height());

            _half_opaque_quad->frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::HALF_OPAQUE_TILE);
            _half_opaque_quad->update(_viewport->get_width() / 2.0f, _viewport->get_height() / 2.0f);

            _text_entity_ids.paused = _text_buffer->create_text();
            _text_entity_ids.controls = _text_buffer->create_text();

            {
                const float text_width = std::strlen(PAUSED_MSG) * 16.0f;
                const float text_center_x = (_viewport->get_width() / 2.0f) - (text_width / 2.0f) + (16.0f / 2);
                const float text_center_y = _viewport->get_height() * 0.8f;

                _text_buffer->update_text(_text_entity_ids.paused, {text_center_x, text_center_y}, PAUSED_MSG, std::strlen(PAUSED_MSG));
            }

            {
                const float text_width = std::strlen(Input::get_controls_msg()) * 16.0f;
                const float text_center_x = (_viewport->get_width() / 2.0f) - (text_width / 2.0f) + (16.0f / 2);
                const float text_center_y = _viewport->get_height() * 0.9f;

                _text_buffer->update_text(_text_entity_ids.controls, {text_center_x, text_center_y}, Input::get_controls_msg(), std::strlen(Input::get_controls_msg()));
            }

        }
        else if (_paused && _last_pause_check)
        {
            _paused = false;
            _half_opaque_quad = nullptr;
            _text_buffer->remove_text(_text_entity_ids.paused);
            _text_buffer->remove_text(_text_entity_ids.controls);
            _text_entity_ids.paused = 0;
        }
    }
    else if (_paused)
    {
        _quit_requested = input.quit_requested();
        _death_requested = input.death_requested();
    }
}

PausePlayingGame::PausePlayingGame(std::shared_ptr<Viewport> viewport) : _viewport(std::move(viewport))
{
}

PausePlayingGame::~PausePlayingGame()
{
    _text_buffer->remove_text(_text_entity_ids.paused);
    _text_buffer->remove_text(_text_entity_ids.controls);
}

void PausePlayingGame::unpause()
{
    _paused = false;
    _last_pause_check = false;
}
