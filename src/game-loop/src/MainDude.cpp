//
// Created by dbeef on 5/7/20.
//

#include "main-dude/MainDude.hpp"
#include "MapTileType.hpp"
#include "Renderer.hpp"
#include <cstring>
#include <main-dude/MainDudeSpritesheetFrames.hpp>

namespace
{
    const float MAIN_DUDE_WIDTH = 1.0f;
    const float MAIN_DUDE_HEIGHT = 1.0f;
}

MainDude::MainDude() : _physics_component(MAIN_DUDE_WIDTH, MAIN_DUDE_HEIGHT)
{
    _x = 3;
    _y = 3;

    const auto texture_region = TextureBank::instance().get_region(TextureType::MAIN_DUDE, static_cast<std::size_t>(MainDudeSpritesheetFrames::STAND_LEFT));

    float offset_x = _x - _physics_component.get_width() / 2;
    float offset_y = _y - _physics_component.get_height() / 2;

    _mesh = texture_region.get_quad_mesh(offset_x, offset_y, true, false);
    _indices = texture_region.get_quad_indices();

    // Render entity

    _render_entity.indices_count = _indices.size();
    _render_entity.indices = _indices.data();
    _render_entity.vertices = _mesh.data();
    _render_entity.texture = TextureBank::instance().get_texture(TextureType::MAIN_DUDE);

    _render_entity.id = Renderer::instance().add_entity(_render_entity);
}

MainDude::~MainDude()
{
    if (_render_entity.id != Renderer::INVALID_ENTITY)
    {
        Renderer::instance().mark_for_removal(_render_entity.id);
    }
}

void MainDude::update(uint32_t delta_time_ms)
{
    _physics_component.update(*this, delta_time_ms);
    _input_component.update(*this);

    _animation_update_delta_ms += delta_time_ms;

    bool vflip = false;

    // TODO: State pattern

    switch (_state)
    {
        case MainDudeState::STANDING_LEFT:
        {
            _current_frame = MainDudeSpritesheetFrames::STAND_LEFT;
            break;
        }
        case MainDudeState::STANDING_RIGHT:
        {
            _current_frame = MainDudeSpritesheetFrames::STAND_LEFT;
            vflip = true;
            break;
        }
        case MainDudeState::RUNNING_LEFT:
        {
            if (_physics_component.get_x_velocity() == 0)
            {
                _state = MainDudeState::STANDING_LEFT;
                break;
            }

            if (_animation_update_delta_ms < 75)
            {
                break;
            }

            bool running_animation_on = _current_frame >= MainDudeSpritesheetFrames::RUN_LEFT_0_FIRST &&
                                        _current_frame <= MainDudeSpritesheetFrames::RUN_LEFT_5_LAST;
            bool last_frame = _current_frame == MainDudeSpritesheetFrames::RUN_LEFT_5_LAST;

            if (!running_animation_on || last_frame)
            {
                _current_frame = MainDudeSpritesheetFrames ::RUN_LEFT_0_FIRST;
            }
            else
            {
                _current_frame = static_cast<MainDudeSpritesheetFrames>(static_cast<int>(_current_frame) + 1);
            }

            _animation_update_delta_ms = 0;


            break;
        }
        case MainDudeState::RUNNING_RIGHT:
        {
            if (_physics_component.get_x_velocity() == 0)
            {
                _state = MainDudeState::STANDING_RIGHT;
            }
            vflip = true;

            if (_animation_update_delta_ms < 75)
            {
                break;
            }

            bool running_animation_on = _current_frame >= MainDudeSpritesheetFrames::RUN_LEFT_0_FIRST &&
                                        _current_frame <= MainDudeSpritesheetFrames::RUN_LEFT_5_LAST;
            bool last_frame = _current_frame == MainDudeSpritesheetFrames::RUN_LEFT_5_LAST;

            if (!running_animation_on || last_frame)
            {
                _current_frame = MainDudeSpritesheetFrames ::RUN_LEFT_0_FIRST;
            }
            else
            {
                _current_frame = static_cast<MainDudeSpritesheetFrames>(static_cast<int>(_current_frame) + 1);
            }

            _animation_update_delta_ms = 0;

            break;
        }
    }

    // Update render entity:
    // TODO: Util for only updating position

    float offset_x = _x - _physics_component.get_width() / 2;
    float offset_y = _y - _physics_component.get_height() / 2;

    // TODO: Dirty flag

    const auto texture_region = TextureBank::instance().get_region(TextureType::MAIN_DUDE, static_cast<std::size_t>(_current_frame));
    const auto new_mesh = texture_region.get_quad_mesh(offset_x, offset_y, vflip, false);
    std::memcpy(_mesh.data(), new_mesh.data(), new_mesh.size() * sizeof(Vertex));
}

