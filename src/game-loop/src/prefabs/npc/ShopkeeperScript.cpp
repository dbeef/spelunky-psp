#include "prefabs/npc/ShopkeeperScript.hpp"
#include "prefabs/items/Shotgun.hpp"

#include "components/damage/HitpointComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/damage/GiveNpcTouchDamageComponent.hpp"

#include "other/ParticleGenerator.hpp"

void prefabs::ShopkeeperJumpOnTopDamageObserver::on_notify(const JumpOnTopDamage_t*)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& position = registry.get<PositionComponent>(_shopkeeper);
    auto& hitpoints = registry.get<HitpointComponent>(_shopkeeper);

    if (hitpoints.get_hitpoints() <= 0)
    {
        // Already handled by ShopkeeperDeathObserver
        return;
    }

    ParticleGenerator().particle_type(ParticleType::BLOOD)
            .position(position.x_center, position.y_center)
            .max_velocity(0.25f, 0.25f)
            .quantity(4)
            .finalize();

    auto& scripting_component = registry.get<ScriptingComponent>(_shopkeeper);
    auto* shopkeeper_script = scripting_component.get<prefabs::ShopkeeperScript>();
    shopkeeper_script->get_angry(_shopkeeper);
}

void prefabs::ShopkeeperMeleeDamageObserver::on_notify(const MeleeDamage_t *)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& position = registry.get<PositionComponent>(_shopkeeper);
    auto& hitpoints = registry.get<HitpointComponent>(_shopkeeper);
    auto& physics = registry.get<PhysicsComponent>(_shopkeeper);

    if (hitpoints.get_hitpoints() <= 0)
    {
        // Already handled by ShopkeeperDeathObserver
        return;
    }

    ParticleGenerator().particle_type(ParticleType::BLOOD)
            .position(position.x_center, position.y_center)
            .max_velocity(0.25f, 0.25f)
            .quantity(4)
            .finalize();

    physics.add_velocity(0.0f, -0.05f);

    auto& scripting_component = registry.get<ScriptingComponent>(_shopkeeper);
    auto* shopkeeper_script = scripting_component.get<prefabs::ShopkeeperScript>();
    shopkeeper_script->get_angry(_shopkeeper);
}

void prefabs::ShopkeeperProjectileDamageObserver::on_notify(const ProjectileDamage_t *)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& position = registry.get<PositionComponent>(_shopkeeper);
    auto& hitpoints = registry.get<HitpointComponent>(_shopkeeper);

    if (hitpoints.get_hitpoints() <= 0)
    {
        // Already handled by ShopkeeperDeathObserver
        return;
    }

    ParticleGenerator().particle_type(ParticleType::BLOOD)
            .position(position.x_center, position.y_center)
            .max_velocity(0.25f, 0.25f)
            .quantity(4)
            .finalize();

    auto& scripting_component = registry.get<ScriptingComponent>(_shopkeeper);
    auto* shopkeeper_script = scripting_component.get<prefabs::ShopkeeperScript>();
    shopkeeper_script->get_angry(_shopkeeper);
    shopkeeper_script->enter_state(&shopkeeper_script->_states.stunned, _shopkeeper);
}

void prefabs::ShopkeeperDeathObserver::on_notify(const DeathEvent *)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& position = registry.get<PositionComponent>(_shopkeeper);

    ParticleGenerator().particle_type(ParticleType::BLOOD)
            .position(position.x_center, position.y_center)
            .max_velocity(0.25f, 0.25f)
            .quantity(4)
            .finalize();

    auto& scripting_component = registry.get<ScriptingComponent>(_shopkeeper);
    auto* shopkeeper_script = scripting_component.get<prefabs::ShopkeeperScript>();
    shopkeeper_script->enter_state(&shopkeeper_script->_states.dead, _shopkeeper);

    // TODO: If killed by main dude, set some permanent flag to indicate that other shopkeepers
    //       should be automatically hostile to main dude.
}

void prefabs::ShopkeeperScript::update(entt::entity owner, uint32_t delta_time_ms)
{
    auto new_state = _states.current->update(*this, delta_time_ms, owner);
    enter_state(new_state, owner);
}

void prefabs::ShopkeeperScript::enter_state(ShopkeeperBaseState* new_state, entt::entity owner)
{
    if (new_state != _states.current)
    {
        _states.current->exit(*this, owner);
        _states.current = new_state;
        _states.current->enter(*this, owner);
    }
}

void prefabs::ShopkeeperScript::get_angry(entt::entity shopkeeper)
{
    if (_angry)
    {
        return;
    }

    auto& registry = EntityRegistry::instance().get_registry();
    auto& position = registry.get<PositionComponent>(shopkeeper);
    auto& item_carrier = registry.get<ItemCarrierComponent>(shopkeeper);

    auto shotgun = prefabs::Shotgun::create(position.x_center, position.y_center);
    item_carrier.pick_up_item(shotgun, shopkeeper);

    registry.emplace<GiveNpcTouchDamageComponent>(shopkeeper);

    _angry = true;
}
