#include "EntityRegistry.hpp"
#include "main-dude/states/MainDudeJumpingState.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "Input.hpp"
#include "audio/Audio.hpp"

void MainDudeJumpingState::enter(MainDudeComponent& dude)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& animation = registry.get<AnimationComponent>(owner);
    auto& quad = registry.get<QuadComponent>(owner);

    animation.stop();
    quad.frame_changed(MainDudeSpritesheetFrames::JUMP_LEFT);

    Audio::instance().play(SFXType::MAIN_DUDE_JUMP);
}

MainDudeBaseState* MainDudeJumpingState::update(MainDudeComponent& dude, uint32_t delta_time_ms)
{
    const auto& input = Input::instance();
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& animation = registry.get<AnimationComponent>(owner);
    auto& physics = registry.get<PhysicsComponent>(owner);
    auto& quad = registry.get<QuadComponent>(owner);
    auto& position = registry.get<PositionComponent>(owner);

    if (input.left().value())
    {
        physics.set_x_velocity(physics.get_x_velocity() - MainDudeComponent::DEFAULT_DELTA_X);
        if (dude.hang_off_cliff_left(physics, position))
        {
            return &dude._states.cliff_hanging;
        }
    }

    if (input.right().value())
    {
        physics.set_x_velocity(physics.get_x_velocity() + MainDudeComponent::DEFAULT_DELTA_X);
        if (dude.hang_off_cliff_right(physics, position))
        {
            return &dude._states.cliff_hanging;
        }
    }

    if (input.running_fast().value())
    {
        physics.set_max_x_velocity(MainDudeComponent::MAX_RUNNING_VELOCITY_X);
        animation.set_time_per_frame_ms(50);
    }
    else
    {
        physics.set_max_x_velocity(MainDudeComponent::DEFAULT_MAX_X_VELOCITY);
        animation.set_time_per_frame_ms(75);
    }

    if (input.throwing().changed() && input.throwing().value())
    {
        return &dude._states.throwing;
    }

    if (input.up().value())
    {
        const auto* exit_tile = dude.is_overlaping_tile(MapTileType::EXIT, physics, position);
        if (exit_tile)
        {
            position.set_position_on_tile(exit_tile);
            return &dude._states.exiting;
        }

        const auto* ladder_tile = dude.is_overlaping_tile(MapTileType::LADDER, physics, position);
        const auto* ladder_deck_tile = dude.is_overlaping_tile(MapTileType::LADDER_DECK, physics, position);

        if (ladder_tile || ladder_deck_tile)
        {
            const auto* tile = ladder_tile ? ladder_tile : ladder_deck_tile;
            position.x_center = tile->x + quad.get_quad_width() / 2;

            return &dude._states.climbing;
        }
    }

    if (physics.is_bottom_collision())
    {
        if (physics.get_x_velocity() == 0.0f)
        {
            return &dude._states.standing;
        }
        else
        {
            return &dude._states.running;
        }
    }
    else if (physics.get_y_velocity() > 0.0f)
    {
        return &dude._states.falling;
    }

    return this;
}
