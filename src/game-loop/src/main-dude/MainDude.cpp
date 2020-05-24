#include <main-dude/MainDude.hpp>
#include "LevelGenerator.hpp"
#include "Collisions.hpp"
#include "main-dude/MainDude.hpp"
#include "MapTileType.hpp"
#include "Renderer.hpp"
#include "Input.hpp"
#include "main-dude/MainDudeSpritesheetFrames.hpp"
#include "main-dude/states/MainDudeRunningState.hpp"
#include "main-dude/states/MainDudeStandingState.hpp"

namespace
{
    // Main dude sprites are of 16x16 pixels, but the main dude fits
    // into the portion with 2px margin on the left and right side of a sprite,
    // thus making collision box smaller:
    const float MAIN_DUDE_PHYSICAL_WIDTH = 1.0f - 2 * (2.0f / 16.0f);
    const float MAIN_DUDE_PHYSICAL_HEIGHT = 1.0f;

    const float MAIN_DUDE_QUAD_WIDTH = 1.0f;
    const float MAIN_DUDE_QUAD_HEIGHT = 1.0f;

    const float MAIN_DUDE_MAX_Y_VELOCITY = 0.26f;
}

MainDude::MainDude(float x_pos_center, float y_pos_center)
    : _physics(MAIN_DUDE_PHYSICAL_WIDTH, MAIN_DUDE_PHYSICAL_HEIGHT)
    , _quad(TextureType::MAIN_DUDE, MAIN_DUDE_QUAD_WIDTH, MAIN_DUDE_QUAD_HEIGHT)
{
    _states.current = &_states.standing;
    _states.current->enter(*this);

    _physics.set_position(x_pos_center, y_pos_center);
    _physics.set_max_y_velocity(MAIN_DUDE_MAX_Y_VELOCITY);

    _other.facing_left = true;

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

MapTile* MainDude::is_overlaping_exit() const
{
    MapTile* exit;
    LevelGenerator::instance().getLevel().get_first_tile_of_given_type(MapTileType::EXIT, exit);
    if (exit)
    {
        if (collisions::overlaps(exit, _physics.get_x_position(), _physics.get_y_position(), _physics.get_width(), _physics.get_height()))
        {
            return exit;
        }
        else
        {
            return nullptr;
        }
    }
    else
    {
        return nullptr;
    }
}

void MainDude::set_position_on_tile(MapTile *map_tile)
{
    const auto x = map_tile->x + _quad.get_quad_width() / 2;
    const auto y = map_tile->y + _quad.get_quad_height() / 2;
    _physics.set_position(x, y);
}

void MainDude::enter_level_summary_state()
{
    _states.current = &_states.level_summary;
    _states.current->enter(*this);
}
