#include "EntityRegistry.hpp"
#include "main-dude/states/MainDudeFallingState.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "Input.hpp"
#include "other/Inventory.hpp"

void MainDudeFallingState::enter(MainDudeComponent& dude)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& animation = registry.get<AnimationComponent>(owner);
    auto& physics = registry.get<PhysicsComponent>(owner);
    auto& quad = registry.get<QuadComponent>(owner);

    animation.stop();
    quad.frame_changed(MainDudeSpritesheetFrames::JUMP_LEFT);

    _last_y_speed = 0.0f;
}

MainDudeBaseState* MainDudeFallingState::update(MainDudeComponent& dude, uint32_t delta_time_ms)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    const auto& input = Input::instance();

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
        if (_last_y_speed >= MainDudeComponent::DEFAULT_MAX_Y_VELOCITY)
        {
            Inventory::instance().remove_hearts(1);

            if (Inventory::instance().get_hearts() == 0)
            {
                return &dude._states.dead;
            }
            else
            {
                return &dude._states.stunned;
            }
        }
        else if (physics.get_x_velocity() == 0.0f)
        {
            return &dude._states.standing;
        }
        else
        {
            return &dude._states.running;
        }
    }

    _last_y_speed = physics.get_y_velocity();

    return this;
}
