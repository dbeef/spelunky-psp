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

MainDude::MainDude()
    : _physics_component(MAIN_DUDE_WIDTH, MAIN_DUDE_HEIGHT)
    , _quad_component(TextureType::MAIN_DUDE)
{
    _states.current = &_states.standing;
     _states.current->enter(*this);

    _x = 3;
    _y = 3;
}

MainDude::~MainDude()
{
    if (_render_entity_id != Renderer::INVALID_ENTITY)
    {
        Renderer::instance().mark_for_removal(_render_entity_id);
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

