#include "Level.hpp"
#include "main-dude/MainDude.hpp"
#include "main-dude/states/MainDudeClimbingLadderState.hpp"
#include "Input.hpp"
#include "audio/Audio.hpp"
#include "other/World.hpp"
#include "TileBatch.hpp"

namespace
{
    bool is_end_of_ladder(World* world, MapTile *tile)
    {
        if (tile->y - 1 >= 0)
        {
            auto* tile_above = world->get_tile_batch()->map_tiles[tile->x][tile->y - 1];
            return tile_above->map_tile_type != MapTileType::LADDER && tile_above->map_tile_type != MapTileType::LADDER_DECK;
        }
        else
        {
            return true;
        }
    }
}

void MainDudeClimbingLadderState::enter(MainDude& main_dude)
{
    auto* physics = main_dude.get_physics_component();
    auto* quad = main_dude.get_quad_component();
    auto* animation = main_dude.get_animation_component();

    assert(physics);
    assert(quad);
    assert(animation);

    physics->set_velocity(0.0f, 0.0f);
    physics->disable_gravity();
    quad->frame_changed(MainDudeSpritesheetFrames::CLIMBING_LADDER_0_FIRST);
    animation->start(static_cast<std::size_t>(MainDudeSpritesheetFrames::CLIMBING_LADDER_0_FIRST),
                               static_cast<std::size_t>(MainDudeSpritesheetFrames::CLIMBING_LADDER_5_LAST),
                               75, true);
}

MainDudeBaseState* MainDudeClimbingLadderState::update(MainDude& main_dude, World* world, uint32_t delta_time_ms)
{
    auto* physics = main_dude.get_physics_component();
    auto* quad = main_dude.get_quad_component();
    auto* animation = main_dude.get_animation_component();

    assert(physics);
    assert(quad);
    assert(animation);

    physics->update(world, delta_time_ms);
    quad->update(physics->get_x_position(), physics->get_y_position(), !main_dude._other.facing_left);

    if (physics->get_y_velocity() != 0.0f)
    {
        animation->update(*quad, delta_time_ms);
    }

    _climbing_ladder_timer += delta_time_ms;
    if (_climbing_ladder_timer > 250)
    {
        _climbing_ladder_sound_toggle = !_climbing_ladder_sound_toggle;
        _climbing_ladder_sound_playing = false;
    }

    return this;
}

MainDudeBaseState *MainDudeClimbingLadderState::handle_input(MainDude& main_dude, World* world, const Input &input)
{
    auto* physics = main_dude.get_physics_component();
    auto* quad = main_dude.get_quad_component();
    auto* animation = main_dude.get_animation_component();

    assert(physics);
    assert(quad);
    assert(animation);

    if (input.jumping().changed() && input.jumping().value())
    {
        physics->add_velocity(0.0f, -MainDude::JUMP_SPEED);
        return &main_dude._states.jumping;
    }

    const auto ladder_tile = main_dude.is_overlaping_tile(world, MapTileType::LADDER);
    const auto ladder_deck_tile = main_dude.is_overlaping_tile(world, MapTileType::LADDER_DECK);

    if (ladder_tile || ladder_deck_tile)
    {
        if (input.up().value())
        {
            physics->set_velocity(0.0f, -0.025f);
            play_sound();
        }
        else if (input.down().value())
        {
            physics->set_velocity(0.0f, 0.025f);
            play_sound();

            if (physics->is_bottom_collision())
            {
                return &main_dude._states.standing;
            }
        }
        else
        {
            physics->set_velocity(0.0f, 0.0f);
        }
    }

    // Ladders are always topped with MapTileType::LADDER tiles, therefore checking only for this type:
    if (ladder_tile && is_end_of_ladder(world, ladder_tile))
    {
        if (physics->get_y_position() <= ladder_tile->y)
        {
            physics->set_position(physics->get_x_position(), ladder_tile->y);
            // Prohibit further climbing upwards:
            if (physics->get_y_velocity() < 0.0f)
            {
                physics->set_velocity(0.0f, 0.0f);
            }
        }
    }

    if (!ladder_tile && !ladder_deck_tile)
    {
        return &main_dude._states.falling;
    }

    return this;
}

void MainDudeClimbingLadderState::exit(MainDude& main_dude)
{
    auto* physics = main_dude.get_physics_component();
    assert(physics);

    physics->enable_gravity();
}

void MainDudeClimbingLadderState::play_sound()
{
    if (_climbing_ladder_sound_playing)
    {
        return;
    }
    else
    {
        _climbing_ladder_timer = 0;
    }

    if (_climbing_ladder_sound_toggle)
    {
        Audio::instance().play(SFXType::MAIN_DUDE_CLIMB_1);
    }
    else
    {
        Audio::instance().play(SFXType::MAIN_DUDE_CLIMB_2);
    }

    _climbing_ladder_sound_playing = true;
}
