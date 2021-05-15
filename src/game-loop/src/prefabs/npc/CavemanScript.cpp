#include "prefabs/npc/CavemanScript.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "other/ParticleGenerator.hpp"
#include "EntityRegistry.hpp"

void prefabs::CavemanProjectileDamageObserver::on_notify(const TakenProjectileDamageEvent *)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& hitpoints = registry.get<HitpointComponent>(_caveman);
    auto& position = registry.get<PositionComponent>(_caveman);

    if (hitpoints.get_hitpoints() <= 0)
    {
        // Already handled by CavemanDeathObserver
        return;
    }

    ParticleGenerator().particle_type(ParticleType::BLOOD)
            .position(position.x_center, position.y_center)
            .max_velocity(0.25f, 0.25f)
            .quantity(2)
            .finalize();

    auto& scripting_component = registry.get<ScriptingComponent>(_caveman);
    auto* caveman_script = scripting_component.get<prefabs::CavemanScript>();
    caveman_script->_angry = true;
    caveman_script->enter_state(&caveman_script->_states.stunned, _caveman);
}

void prefabs::CavemanMeleeDamageObserver::on_notify(const TakenMeleeDamageEvent *)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& hitpoints = registry.get<HitpointComponent>(_caveman);
    auto& physics = registry.get<PhysicsComponent>(_caveman);
    auto& position = registry.get<PositionComponent>(_caveman);

    physics.add_velocity(0.0f, -0.1f);

    if (hitpoints.get_hitpoints() <= 0)
    {
        // Already handled by CavemanDeathObserver
        return;
    }

    ParticleGenerator().particle_type(ParticleType::BLOOD)
            .position(position.x_center, position.y_center)
            .max_velocity(0.25f, 0.25f)
            .quantity(2)
            .finalize();

    auto& scripting_component = registry.get<ScriptingComponent>(_caveman);
    auto* caveman_script = scripting_component.get<prefabs::CavemanScript>();
    caveman_script->_angry = true;
    caveman_script->enter_state(&caveman_script->_states.stunned, _caveman);
}

void prefabs::CavemanJumpOnTopDamageObserver::on_notify(const TakenJumpOnTopDamageEvent *)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& hitpoints = registry.get<HitpointComponent>(_caveman);
    auto& physics = registry.get<PhysicsComponent>(_caveman);
    auto& position = registry.get<PositionComponent>(_caveman);

    if (hitpoints.get_hitpoints() <= 0)
    {
        // Already handled by CavemanDeathObserver
        return;
    }

    ParticleGenerator().particle_type(ParticleType::BLOOD)
            .position(position.x_center, position.y_center)
            .max_velocity(0.25f, 0.25f)
            .quantity(2)
            .finalize();

    auto& scripting_component = registry.get<ScriptingComponent>(_caveman);
    auto* caveman_script = scripting_component.get<prefabs::CavemanScript>();
    caveman_script->_angry = true;
    caveman_script->enter_state(&caveman_script->_states.stunned, _caveman);
}

void prefabs::CavemanDeathObserver::on_notify(const DeathEvent *)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& position = registry.get<PositionComponent>(_caveman);

    ParticleGenerator().particle_type(ParticleType::BLOOD)
            .position(position.x_center, position.y_center)
            .max_velocity(0.25f, 0.25f)
            .quantity(2)
            .finalize();

    auto& scripting_component = registry.get<ScriptingComponent>(_caveman);
    auto* caveman_script = scripting_component.get<prefabs::CavemanScript>();
    caveman_script->enter_state(&caveman_script->_states.dead, _caveman);
}

void prefabs::CavemanScript::enter_state(CavemanBaseState *new_state, entt::entity owner)
{
    if (new_state != _states.current)
    {
        _states.current->exit(*this, owner);
        _states.current = new_state;
        _states.current->enter(*this, owner);
    }
}

void prefabs::CavemanScript::update(entt::entity owner, uint32_t delta_time_ms)
{
    auto new_state = _states.current->update(*this, delta_time_ms, owner);
    enter_state(new_state, owner);
}

void prefabs::CavemanScript::add_take_melee_damage_component(entt::entity caveman)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& scripting_component = registry.get<ScriptingComponent>(caveman);
    auto* caveman_script = scripting_component.get<prefabs::CavemanScript>();

    TakeMeleeDamageComponent take_melee_damage;
    take_melee_damage.add_observer(reinterpret_cast<Observer<TakenMeleeDamageEvent>*>(caveman_script->get_melee_damage_observer()));
    registry.emplace<TakeMeleeDamageComponent>(caveman, take_melee_damage);
}

void prefabs::CavemanScript::add_take_jump_on_top_damage_component(entt::entity caveman)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& scripting_component = registry.get<ScriptingComponent>(caveman);
    auto* caveman_script = scripting_component.get<prefabs::CavemanScript>();

    TakeJumpOnTopDamageComponent take_jump_on_top_damage;
    take_jump_on_top_damage.add_observer(reinterpret_cast<Observer<TakenJumpOnTopDamageEvent>*>(caveman_script->get_jump_on_top_damage_observer()));
    registry.emplace<TakeJumpOnTopDamageComponent>(caveman, take_jump_on_top_damage);
}
