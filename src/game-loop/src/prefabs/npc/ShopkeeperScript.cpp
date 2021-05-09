#include "prefabs/npc/ShopkeeperScript.hpp"
#include "prefabs/items/Shotgun.hpp"

#include "components/damage/HitpointComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/SaleableComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/damage/GiveNpcTouchDamageComponent.hpp"

#include "other/ParticleGenerator.hpp"

#include <cmath>

void prefabs::ShopkeeperThieveryObserver::on_notify(const ThieveryEvent* event)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& scripting_component = registry.get<ScriptingComponent>(_shopkeeper);
    auto* shopkeeper_script = scripting_component.get<prefabs::ShopkeeperScript>();

    shopkeeper_script->set_thief(event->thief);
    shopkeeper_script->notify({shopkeeper_script->_thief});
    shopkeeper_script->get_angry(_shopkeeper);
}

void prefabs::ShopkeeperJumpOnTopDamageObserver::on_notify(const TakenJumpOnTopDamageEvent* event)
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
    shopkeeper_script->set_thief(event->source);
    shopkeeper_script->notify({shopkeeper_script->_thief});
    shopkeeper_script->get_angry(_shopkeeper);
}

void prefabs::ShopkeeperMeleeDamageObserver::on_notify(const TakenMeleeDamageEvent* event)
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

    // If it's an item - try to find item carrier:
    if (registry.has<ItemComponent>(event->source))
    {
        auto& item = registry.get<ItemComponent>(event->source);
        if (item.is_carried())
        {
            shopkeeper_script->set_thief(item.get_item_carrier_entity());
        }
        else
        {
            shopkeeper_script->set_thief(event->source);
        }
    }
    else
    {
        shopkeeper_script->set_thief(event->source);
    }

    shopkeeper_script->notify({shopkeeper_script->_thief});
    shopkeeper_script->get_angry(_shopkeeper);
}

void prefabs::ShopkeeperProjectileDamageObserver::on_notify(const TakenProjectileDamageEvent* event)
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
    shopkeeper_script->set_thief(event->source);
    shopkeeper_script->notify({shopkeeper_script->_thief});
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
    shopkeeper_script->notify({shopkeeper_script->_thief});
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

void prefabs::ShopkeeperScript::follow_thief(entt::entity shopkeeper)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& position = registry.get<PositionComponent>(shopkeeper);
    auto& physics = registry.get<PhysicsComponent>(shopkeeper);

    // Following thief is done, but again, how to detect a thief that does not attack but simply take out some item?
    // Define shop zone? Item distance from original placing?

    if (_thief != entt::null && registry.valid(_thief))
    {
        auto& thief_position = registry.get<PositionComponent>(_thief);
        const auto pos_diff_x = position.x_center - thief_position.x_center;
        if (std::fabs(pos_diff_x) > 0.5f * MapTile::PHYSICAL_WIDTH)
        {
            physics.set_x_velocity(std::copysign(0.12f, -pos_diff_x));
        }
        const auto pos_diff_y = position.y_center - thief_position.y_center;
        if (physics.is_bottom_collision() && std::fabs(pos_diff_y) > 1 * MapTile::PHYSICAL_HEIGHT)
        {
            // Or just randomize in the range of 0.16 - 0.24
            if (std::rand() % 2)
            {
                physics.set_y_velocity(-0.16f);
            }
            else
            {
                physics.set_y_velocity(-0.24f);
            }
        }
    }
}

void prefabs::ShopkeeperScript::follow_customer(entt::entity shopkeeper)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& position = registry.get<PositionComponent>(shopkeeper);
    auto& physics = registry.get<PhysicsComponent>(shopkeeper);

    auto saleable_items = registry.view<ItemComponent, SaleableComponent, PositionComponent>();
    saleable_items.each([&](entt::entity item_for_sale_entity,
        ItemComponent& item,
        SaleableComponent& item_saleable,
        PositionComponent& item_position)
    {
        if (item.is_carried())
        {
            auto &item_carrier_position = registry.get<PositionComponent>(item.get_item_carrier_entity());
            const auto pos_diff_x = position.x_center - item_carrier_position.x_center;
            if (std::fabs(pos_diff_x) > 1 * MapTile::PHYSICAL_WIDTH)
            {
                physics.set_x_velocity(std::copysign(0.05f, -pos_diff_x));
            }
        }
    });
}
