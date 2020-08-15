#include <cmath>

#include "NeighbouringTiles.hpp"
#include "logger/log.h"
#include "Level.hpp"
#include "Collisions.hpp"
#include "main-dude/MainDude.hpp"
#include "MapTileType.hpp"
#include "Renderer.hpp"
#include "Input.hpp"
#include "spritesheet-frames/MainDudeSpritesheetFrames.hpp"
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
}

MainDude::MainDude(float x_pos_center, float y_pos_center)
    : _physics(MAIN_DUDE_PHYSICAL_WIDTH, MAIN_DUDE_PHYSICAL_HEIGHT)
    , _quad(TextureType::MAIN_DUDE, Renderer::EntityType::MODEL_VIEW_SPACE, MAIN_DUDE_QUAD_WIDTH, MAIN_DUDE_QUAD_HEIGHT)
{
    _states.current = &_states.standing;
    _states.current->enter(*this);

    _physics.set_position(x_pos_center, y_pos_center);
    _physics.set_max_y_velocity(MainDude::DEFAULT_MAX_Y_VELOCITY);

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
        _states.current->exit(*this);
        _states.current = new_state;
        _states.current->enter(*this);
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

MapTile* MainDude::is_overlaping_tile(MapTileType type) const
{
    MapTile* neighbours[9] = {nullptr};
    Level::instance().get_tile_batch().get_neighbouring_tiles(_physics.get_x_position(), _physics.get_y_position(), neighbours);

    for (const auto neighbour : neighbours)
    {
        if (neighbour == nullptr || neighbour->map_tile_type != type)
        {
            continue;
        }

        if (collisions::overlaps(neighbour, _physics.get_x_position(), _physics.get_y_position(), _physics.get_width(), _physics.get_height()))
        {
            return neighbour;
        }
    }

    return nullptr;
}

void MainDude::set_position_on_tile(MapTile *map_tile)
{
    const auto x = map_tile->x + _quad.get_quad_width() / 2;
    const auto y = map_tile->y + _quad.get_quad_height() / 2;
    _physics.set_position(x, y);
}

void MainDude::enter_dead_state()
{
    _states.current->exit(*this);
    _states.current = &_states.dead;
    _states.current->enter(*this);
}

void MainDude::enter_level_summary_state()
{
    _states.current->exit(*this);
    _states.current = &_states.level_summary;
    _states.current->enter(*this);
}

bool MainDude::hang_off_cliff_right()
{
    if (_physics.is_right_collision())
    {
        MapTile *neighbours[9] = {nullptr};

        Level::instance().get_tile_batch().get_neighbouring_tiles(
                get_x_pos_center(),
                get_y_pos_center(),
                neighbours);

        auto* right_tile = neighbours[static_cast<int>(NeighbouringTiles::CENTER)];
        auto* right_upper_tile = neighbours[static_cast<int>(NeighbouringTiles::UP_MIDDLE)];

        if (right_tile && right_tile->collidable &&
            (!right_upper_tile || !right_upper_tile->collidable) &&
                // Main dude's center must be in margin of a quarter of a tile from its beginning:
                (get_y_pos_center() >= right_tile->y + 0.25f)
            )
        {
            _physics.set_position(
                    right_tile->x - (_physics.get_width() / 2.0f),
                    right_tile->y + 0.5f);
            return true;
        }
    }

    return false;
}

bool MainDude::hang_off_cliff_left()
{
    if (_physics.is_left_collision())
    {
        MapTile *neighbours[9] = {nullptr};

        Level::instance().get_tile_batch().get_neighbouring_tiles(
                get_x_pos_center(),
                get_y_pos_center(),
                neighbours);
      
        auto* left_tile = neighbours[static_cast<int>(NeighbouringTiles::LEFT_MIDDLE)];
        auto* left_upper_tile = neighbours[static_cast<int>(NeighbouringTiles::LEFT_UP)];
      
        if (left_tile && left_tile->collidable &&
            (!left_upper_tile || !left_upper_tile->collidable) &&
                // Main dude's center must be in margin of a quarter of a tile from its beginning:
                (get_y_pos_center() >= left_tile->y + 0.25f)
            )
        {
            _physics.set_position(
                    left_tile->x + MapTile::PHYSICAL_WIDTH + (_physics.get_width() / 2.0f),
                    left_tile->y + (MapTile::PHYSICAL_HEIGHT / 2.0f));
            return true;
        }
    }

    return false;
}

void MainDude::decrease_hearts(uint8_t amount)
{
    _stats.hearts = std::max<int16_t>(0, _stats.hearts - amount);
    notify(MainDudeEvent::HEARTS_COUNT_CHANGED);
}
