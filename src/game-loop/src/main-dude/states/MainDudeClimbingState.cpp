#include "EntityRegistry.hpp"
#include "Level.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "main-dude/states/MainDudeClimbingState.hpp"
#include "components/generic/InputComponent.hpp"
#include "Input.hpp"
#include "audio/Audio.hpp"

void MainDudeClimbingState::enter(MainDudeComponent& dude)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& physics = registry.get<PhysicsComponent>(owner);
    auto& quad = registry.get<QuadComponent>(owner);
    auto& animation = registry.get<AnimationComponent>(owner);
    auto& input = registry.get<InputComponent>(owner);

    input.allowed_events = {
            InputEvent::UP,
            InputEvent::UP_PRESSED,
            InputEvent::DOWN,
            InputEvent::DOWN_PRESSED,
            InputEvent::JUMPING,
            InputEvent::JUMPING_PRESSED,
            InputEvent::THROWING,
            InputEvent::THROWING_PRESSED,
            InputEvent::OUT_BOMB,
            InputEvent::OUT_BOMB_PRESSED,
            InputEvent::OUT_ROPE,
            InputEvent::OUT_ROPE_PRESSED,
    };

    physics.set_x_velocity(0);
    physics.set_y_velocity(0);
    physics.disable_gravity();

    switch (dude.get_climbing_observer()->get_last_event().event_type)
    {
        case ClimbingEventType::STARTED_CLIMBING_LADDER:
        {
            quad.frame_changed(MainDudeSpritesheetFrames::CLIMBING_LADDER_0_FIRST);
            animation.start(static_cast<std::size_t>(MainDudeSpritesheetFrames::CLIMBING_LADDER_0_FIRST),
                            static_cast<std::size_t>(MainDudeSpritesheetFrames::CLIMBING_LADDER_5_LAST),
                            75, true);
            break;
        }
        case ClimbingEventType::STARTED_CLIMBING_ROPE:
        {
            quad.frame_changed(MainDudeSpritesheetFrames::CLIMBING_ROPE_0_FIRST);
            animation.start(static_cast<std::size_t>(MainDudeSpritesheetFrames::CLIMBING_ROPE_0_FIRST),
                            static_cast<std::size_t>(MainDudeSpritesheetFrames::CLIMBING_ROPE_11_LAST),
                            75, true);
            break;
        }
    }
}

MainDudeBaseState* MainDudeClimbingState::update(MainDudeComponent& dude, uint32_t delta_time_ms)
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
        // FIXME: InputSystem checks for bottom collision which is not true in this case
        physics.set_y_velocity(-MainDudeComponent::JUMP_SPEED);
        return &dude._states.jumping;
    }

    auto* overlapped_tile = Level::instance().get_tile_batch().map_tiles[static_cast<int>(position.x_center)][static_cast<int>(position.y_center)];

    if (overlapped_tile && overlapped_tile->climbable)
    {
        if (input.up().value())
        {
            play_sound();
        }
        else if (input.down().value())
        {
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

    if (!overlapped_tile)
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

void MainDudeClimbingState::exit(MainDudeComponent& dude)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& physics = registry.get<PhysicsComponent>(owner);
    physics.enable_gravity();
}

void MainDudeClimbingState::play_sound()
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
