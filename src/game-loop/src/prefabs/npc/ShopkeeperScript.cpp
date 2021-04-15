#include "prefabs/npc/ShopkeeperScript.hpp"
#include "other/ParticleGenerator.hpp"
#include "components/damage/HitpointComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/PositionComponent.hpp"

void prefabs::ShopkeeperDeathObserver::on_notify(const DeathEvent *)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& position = registry.get<PositionComponent>(_shopkeeper);

    ParticleGenerator().particle_type(ParticleType::BLOOD)
            .position(position.x_center, position.y_center)
            .max_velocity(0.25f, 0.25f)
            .quantity(4)
            .finalize();

    // TODO: If killed by main dude, set some permanent flag to indicate that other shopkeepers
    //       should be automatically hostile to main dude.
}

void prefabs::ShopkeeperScript::update(entt::entity owner, uint32_t delta_time_ms)
{
    auto new_state = _states.current->update(*this, delta_time_ms, owner);
    if (new_state != _states.current)
    {
        _states.current->exit(owner);
        _states.current = new_state;
        _states.current->enter(owner);
    }
}