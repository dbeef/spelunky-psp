#include "prefabs/npc/DamselScript.hpp"
#include "other/ParticleGenerator.hpp"
#include "components/damage/HitpointComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/PositionComponent.hpp"

void prefabs::DamselProjectileDamageObserver::on_notify(const ProjectileDamage_t *)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& hitpoints = registry.get<HitpointComponent>(_damsel);

    if (hitpoints.get_hitpoints() <= 0)
    {
        // Already handled by DamselDeathObserver
        return;
    }

    auto& position = registry.get<PositionComponent>(_damsel);

    ParticleGenerator().particle_type(ParticleType::BLOOD)
            .position(position.x_center, position.y_center)
            .max_velocity(0.25f, 0.25f)
            .quantity(2)
            .finalize();

    auto& scripting_component = registry.get<ScriptingComponent>(_damsel);
    auto* damsel_script = scripting_component.get<prefabs::DamselScript>();
    damsel_script->set_panic(true);
    damsel_script->enter_state(&damsel_script->_states.stunned, _damsel);
}

void prefabs::DamselDeathObserver::on_notify(const DeathEvent *)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& position = registry.get<PositionComponent>(_damsel);

    ParticleGenerator().particle_type(ParticleType::BLOOD)
            .position(position.x_center, position.y_center)
            .max_velocity(0.25f, 0.25f)
            .quantity(2)
            .finalize();

    auto& scripting_component = registry.get<ScriptingComponent>(_damsel);
    auto* damsel_script = scripting_component.get<prefabs::DamselScript>();
    damsel_script->enter_state(&damsel_script->_states.dead, _damsel);
}

void prefabs::DamselScript::enter_state(DamselBaseState* new_state, entt::entity owner)
{
    if (new_state != _states.current)
    {
        _states.current->exit(*this, owner);
        _states.current = new_state;
        _states.current->enter(*this, owner);
    }
}

void prefabs::DamselScript::update(entt::entity owner, uint32_t delta_time_ms)
{
    auto new_state = _states.current->update(*this, delta_time_ms, owner);
    enter_state(new_state, owner);
}
