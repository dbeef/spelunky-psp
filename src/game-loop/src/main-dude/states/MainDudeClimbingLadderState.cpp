#include "EntityRegistry.hpp"
#include "Level.hpp"
#include "components/specialized/MainDudeComponent.hpp"
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

void MainDudeClimbingLadderState::enter(MainDudeComponent& dude)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& physics = registry.get<PhysicsComponent>(owner);
    auto& quad = registry.get<QuadComponent>(owner);
    auto& animation = registry.get<AnimationComponent>(owner);

    physics.set_x_velocity(0);
    physics.set_y_velocity(0);
    physics.disable_gravity();
    quad.frame_changed(MainDudeSpritesheetFrames::CLIMBING_LADDER_0_FIRST);
    animation.start(static_cast<std::size_t>(MainDudeSpritesheetFrames::CLIMBING_LADDER_0_FIRST),
                    static_cast<std::size_t>(MainDudeSpritesheetFrames::CLIMBING_LADDER_5_LAST),
    75, true);
}

MainDudeBaseState* MainDudeClimbingLadderState::update(MainDudeComponent& dude, uint32_t delta_time_ms)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    const auto& input = Input::instance();

    auto& physics = registry.get<PhysicsComponent>(owner);
    auto& quad = registry.get<QuadComponent>(owner);
    auto& animation = registry.get<AnimationComponent>(owner);
    auto& position = registry.get<PositionComponent>(owner);

    if (input.jumping().changed() && input.jumping().value())
    {
        physics.set_y_velocity(physics.get_y_velocity() - MainDudeComponent::JUMP_SPEED);
        return &dude._states.jumping;
    }

    const auto ladder_tile = dude.is_overlaping_tile(MapTileType::LADDER, physics, position);
    const auto ladder_deck_tile = dude.is_overlaping_tile(MapTileType::LADDER_DECK, physics, position);

    if (ladder_tile || ladder_deck_tile)
    {
        if (input.up().value())
        {
            physics.set_x_velocity(0.0f);
            physics.set_y_velocity(-0.025f);
            play_sound();
        }
        else if (input.down().value())
        {
            physics.set_x_velocity(0.0f);
            physics.set_y_velocity(0.025f);
            play_sound();

            if (physics.is_bottom_collision())
            {
                return &dude._states.standing;
            }
        }
        else
        {
            physics.set_x_velocity(0.0f);
            physics.set_y_velocity(0.0f);
        }
    }

    // Ladders are always topped with MapTileType::LADDER tiles, therefore checking only for this type:
    if (ladder_tile && is_end_of_ladder(ladder_tile))
    {
        if (position.y_center <= ladder_tile->y)
        {
            position.y_center = ladder_tile->y;
            // Prohibit further climbing upwards:
            if (physics.get_y_velocity() < 0.0f)
            {
                physics.set_x_velocity(0.0f);
                physics.set_y_velocity(0.0f);
            }
        }
    }

    if (!ladder_tile && !ladder_deck_tile)
    {
        return &dude._states.falling;
    }

    if (physics.get_y_velocity() != 0.0f)
    {
        animation.resume();
    }
    else
    {
        animation.stop();
    }

    _climbing_ladder_timer += delta_time_ms;

    if (_climbing_ladder_timer > 250)
    {
        _climbing_ladder_sound_toggle = !_climbing_ladder_sound_toggle;
        _climbing_ladder_sound_playing = false;
    }

    return this;
}

void MainDudeClimbingLadderState::exit(MainDudeComponent& dude)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& physics = registry.get<PhysicsComponent>(owner);
    physics.enable_gravity();
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
