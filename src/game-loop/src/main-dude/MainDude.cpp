//
// Created by dbeef on 5/7/20.
//

#include <main-dude/MainDude.hpp>
#include "main-dude/MainDude.hpp"
#include "MapTileType.hpp"
#include "Renderer.hpp"
#include "main-dude/MainDudeSpritesheetFrames.hpp"
#include "main-dude/states/MainDudeRunning.hpp"
#include "main-dude/states/MainDudeStanding.hpp"

namespace
{
    const float MAIN_DUDE_WIDTH = 1.0f;
    const float MAIN_DUDE_HEIGHT = 1.0f;
}

MainDude::MainDude() : _physics_component(MAIN_DUDE_WIDTH, MAIN_DUDE_HEIGHT)
{
    _states.current = &_states.standing;

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
    assert(_states.current);
    auto new_state = _states.current->update(*this, delta_time_ms);

    if (new_state != _states.current)
    {
        new_state->enter(*this);
        _states.current = new_state;
    }
}

