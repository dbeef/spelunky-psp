#include "EntityRegistry.hpp"
#include "main-dude/states/MainDudeExitingState.hpp"
#include "Input.hpp"
#include "audio/Audio.hpp"
#include "EntityRegistry.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "components/generic/InputComponent.hpp"
#include "components/damage/TakeMeleeDamageComponent.hpp"
#include "components/damage/TakeExplosionDamageComponent.hpp"
#include "components/damage/TakeProjectileDamageComponent.hpp"
#include "components/damage/TakeNpcTouchDamageComponent.hpp"

void MainDudeExitingState::exit(MainDudeComponent& dude)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& physics = registry.get<PhysicsComponent>(dude._owner);
    physics.enable_gravity();

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
    physics.disable_gravity();

    input.allowed_events = {};

    Audio::instance().play(SFXType::MAIN_DUDE_ENTERING_DOOR);

    auto& animation = registry.get<AnimationComponent>(owner);
    animation.start(static_cast<std::size_t>(MainDudeSpritesheetFrames::EXITING_LEFT_0_FIRST),
                    static_cast<std::size_t>(MainDudeSpritesheetFrames::EXITING_LEFT_15_LAST),
                    75, false);

    if (registry.has<TakeMeleeDamageComponent>(owner)) registry.remove<TakeMeleeDamageComponent>(owner);
    if (registry.has<TakeExplosionDamageComponent>(owner)) registry.remove<TakeExplosionDamageComponent>(owner);
    if (registry.has<TakeProjectileDamageComponent>(owner)) registry.remove<TakeProjectileDamageComponent>(owner);
    if (registry.has<TakeNpcTouchDamageComponent>(owner)) registry.remove<TakeNpcTouchDamageComponent>(owner);
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
