#include <logger/log.h>
#include <main-dude/MainDude.hpp>
#include "main-dude/MainDude.hpp"
#include "MapTileType.hpp"
#include "Renderer.hpp"
#include "Input.hpp"
#include "main-dude/MainDudeSpritesheetFrames.hpp"
#include "main-dude/states/MainDudeRunningState.hpp"
#include "main-dude/states/MainDudeStandingState.hpp"

namespace
{
    const float MAIN_DUDE_WIDTH = 1.0f;
    const float MAIN_DUDE_HEIGHT = 1.0f;
    const float MAIN_DUDE_MAX_Y_VELOCITY = 0.128f;
}

MainDude::MainDude()
    : _physics(MAIN_DUDE_WIDTH, MAIN_DUDE_HEIGHT)
    , _quad(TextureType::MAIN_DUDE)
{
    _states.current = &_states.standing;
    _states.current->enter(*this);

    _physics.set_position(3, 3);
    _physics.set_max_y_velocity(MAIN_DUDE_MAX_Y_VELOCITY);

    update(0);
}

void MainDude::update(uint32_t delta_time_ms)
{
    // Update generic properties:
    
    if (_physics.get_x_velocity() != 0.0f)
    {
        _other.facing_left = _physics.get_x_velocity() < 0.0f;
        _quad.frame_changed();
    }

    // Handle input

    handle_input(Input::instance());

    // Update current state:
    
    assert(_states.current);
    auto new_state = _states.current->update(*this, delta_time_ms);

    if (new_state != _states.current)
    {
        new_state->enter(*this);
        _states.current = new_state;
    }
}

void MainDude::handle_input(const Input &input)
{
    assert(_states.current);
    auto new_state = _states.current->handle_input(*this, input);

    if (new_state != _states.current)
    {
        new_state->enter(*this);
        _states.current = new_state;
    }
}

