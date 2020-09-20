#include "Level.hpp"
#include "main-dude/MainDude.hpp"
#include "main-dude/states/MainDudeClimbingLadderState.hpp"
#include "Input.hpp"
#include "audio/Audio.hpp"

namespace
{
    bool is_end_of_ladder(MapTile *tile)
    {
        if (tile->y - 1 >= 0)
        {
            auto* tile_above = Level::instance().get_tile_batch().map_tiles[tile->x][tile->y - 1];
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
    main_dude._physics.set_velocity(0.0f, 0.0f);
    main_dude._physics.disable_gravity();
    main_dude._quad.frame_changed(MainDudeSpritesheetFrames::CLIMBING_LADDER_0_FIRST);
    main_dude._animation.start(static_cast<std::size_t>(MainDudeSpritesheetFrames::CLIMBING_LADDER_0_FIRST),
                               static_cast<std::size_t>(MainDudeSpritesheetFrames::CLIMBING_LADDER_5_LAST),
                               75, true);
}

MainDudeBaseState* MainDudeClimbingLadderState::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // Update components:
    main_dude._physics.update(delta_time_ms);
    main_dude._quad.update(main_dude.get_x_pos_center(), main_dude.get_y_pos_center(), !main_dude._other.facing_left);

    if (main_dude._physics.get_y_velocity() != 0.0f)
    {
        main_dude._animation.update(main_dude, delta_time_ms);
    }

    _climbing_ladder_timer += delta_time_ms;
    if (_climbing_ladder_timer > 250)
    {
        _climbing_ladder_sound_toggle = !_climbing_ladder_sound_toggle;
        _climbing_ladder_sound_playing = false;
    }

    return this;
}

MainDudeBaseState *MainDudeClimbingLadderState::handle_input(MainDude& main_dude, const Input &input)
{
    if (input.jumping().changed() && input.jumping().value())
    {
        main_dude._physics.add_velocity(0.0f, -MainDude::JUMP_SPEED);
        return &main_dude._states.jumping;
    }

    const auto ladder_tile = main_dude.is_overlaping_tile(MapTileType::LADDER);
    const auto ladder_deck_tile = main_dude.is_overlaping_tile(MapTileType::LADDER_DECK);

    if (ladder_tile || ladder_deck_tile)
    {
        if (input.up().value())
        {
            main_dude._physics.set_velocity(0.0f, -0.025f);
            play_sound();
        }
        else if (input.down().value())
        {
            main_dude._physics.set_velocity(0.0f, 0.025f);
            play_sound();

            if (main_dude._physics.is_bottom_collision())
            {
                return &main_dude._states.standing;
            }
        }
        else
        {
            main_dude._physics.set_velocity(0.0f, 0.0f);
        }
    }

    // Ladders are always topped with MapTileType::LADDER tiles, therefore checking only for this type:
    if (ladder_tile && is_end_of_ladder(ladder_tile))
    {
        if (main_dude._physics.get_y_position() <= ladder_tile->y)
        {
            main_dude._physics.set_position(main_dude.get_x_pos_center(), ladder_tile->y);
            // Prohibit further climbing upwards:
            if (main_dude._physics.get_y_velocity() < 0.0f)
            {
                main_dude._physics.set_velocity(0.0f, 0.0f);
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
    main_dude._physics.enable_gravity();
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
