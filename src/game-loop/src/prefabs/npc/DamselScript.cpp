#include "prefabs/npc/DamselScript.hpp"
#include "other/ParticleGenerator.hpp"
#include "components/damage/HitpointComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/PositionComponent.hpp"

void prefabs::DamselDeathObserver::on_notify(const DeathEvent *)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& position = registry.get<PositionComponent>(_damsel);

    ParticleGenerator().particle_type(ParticleType::BLOOD)
            .position(position.x_center, position.y_center)
            .max_velocity(0.25f, 0.25f)
            .quantity(4)
            .finalize();

    auto& scripting_component = registry.get<ScriptingComponent>(_damsel);
    auto* damsel_script = scripting_component.get<prefabs::DamselScript>();
    damsel_script->enter_state(&damsel_script->_states.dead, _damsel);
}

void prefabs::DamselScript::enter_state(DamselBaseState* new_state, entt::entity owner)
{
    if (new_state != _states.current)
    {
        _states.current->exit(owner);
        _states.current = new_state;
        _states.current->enter(owner);
    }
}

void prefabs::DamselScript::update(entt::entity owner, uint32_t delta_time_ms)
{
    auto new_state = _states.current->update(*this, delta_time_ms, owner);
    enter_state(new_state, owner);
}
