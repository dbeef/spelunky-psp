#include <LevelGenerator.hpp>
#include <logger/log.h>
#include <main-dude/MainDude.hpp>
#include "main-dude/states/MainDudeClimbingLadderState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

namespace
{
    bool is_end_of_ladder(MapTile *tile)
    {
        if (tile->y - 1 >= 0)
        {
            auto* tile_above = LevelGenerator::instance().getLevel().map_tiles[tile->x][tile->y - 1];
            return tile_above->mapTileType != MapTileType::LADDER && tile_above->mapTileType != MapTileType::LADDER_DECK;
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

    main_dude._physics.update(main_dude, delta_time_ms);
    main_dude._quad.update(main_dude, delta_time_ms);
    if (main_dude._physics.get_y_velocity() != 0.0f)
    {
        main_dude._animation.update(main_dude, delta_time_ms);
    }

    // Other:

    return this;
}

MainDudeBaseState *MainDudeClimbingLadderState::handle_input(MainDude& main_dude, const Input &input)
{
    if (input.circle())
    {
        main_dude._physics.add_velocity(0.0f, -MainDude::JUMP_SPEED);
        main_dude._physics.enable_gravity();
        return &main_dude._states.jumping;
    }

    const auto ladder_tile = main_dude.is_overlaping_tile(MapTileType::LADDER);
    const auto ladder_deck_tile = main_dude.is_overlaping_tile(MapTileType::LADDER_DECK);

    if (ladder_tile || ladder_deck_tile)
    {
        if (input.up())
        {
            main_dude._physics.set_velocity(0.0f, -0.025f);
        }
        else if (input.down())
        {
            main_dude._physics.set_velocity(0.0f, 0.025f);

            if (main_dude._physics.is_bottom_collision())
            {
                main_dude._physics.enable_gravity();
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
            if (main_dude._physics.get_y_velocity() < 0.0f)
            {
                main_dude._physics.set_velocity(0.0f, 0.0f);
            }
        }
    }

    if (!ladder_tile && !ladder_deck_tile)
    {
        main_dude._physics.enable_gravity();
        return &main_dude._states.falling;
    }

    return this;
}
