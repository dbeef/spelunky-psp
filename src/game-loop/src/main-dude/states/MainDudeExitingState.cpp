#include <components/generic/InputComponent.hpp>
#include "EntityRegistry.hpp"
#include "main-dude/states/MainDudeExitingState.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "Input.hpp"
#include "audio/Audio.hpp"
#include "EntityRegistry.hpp"

void MainDudeExitingState::exit(MainDudeComponent& dude)
{
    dude._other.entered_door = false;
}

void MainDudeExitingState::enter(MainDudeComponent& dude)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& input = registry.get<InputComponent>(owner);
    auto& physics = registry.get<PhysicsComponent>(owner);

    physics.set_x_velocity(0);
    physics.set_y_velocity(0);
    input.allowed_events = {};

    Audio::instance().play(SFXType::MAIN_DUDE_ENTERING_DOOR);

    auto& animation = registry.get<AnimationComponent>(owner);
    animation.start(static_cast<std::size_t>(MainDudeSpritesheetFrames::EXITING_LEFT_0_FIRST),
                    static_cast<std::size_t>(MainDudeSpritesheetFrames::EXITING_LEFT_15_LAST),
                    75, false);
}

MainDudeBaseState* MainDudeExitingState::update(MainDudeComponent& dude, uint32_t delta_time_ms)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& animation = registry.get<AnimationComponent>(owner);
    auto& quad = registry.get<QuadComponent>(owner);

    if (animation.is_finished())
    {
        dude._other.entered_door = true;
    }

    return this;
}
