#include "prefabs/npc/ShopkeeperScript.hpp"
#include "prefabs/items/Shotgun.hpp"

#include "components/damage/HitpointComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/SaleableComponent.hpp"
#include "components/generic/ActivableComponent.hpp"
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

    shopkeeper_script->notify(ShopkeeperAssaultEvent{});
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
            .quantity(2)
            .finalize();

    auto& scripting_component = registry.get<ScriptingComponent>(_shopkeeper);
    auto* shopkeeper_script = scripting_component.get<prefabs::ShopkeeperScript>();
    shopkeeper_script->notify(ShopkeeperAssaultEvent{});
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
            .quantity(2)
            .finalize();

    physics.add_velocity(0.0f, -0.05f);

    auto& scripting_component = registry.get<ScriptingComponent>(_shopkeeper);
    auto* shopkeeper_script = scripting_component.get<prefabs::ShopkeeperScript>();

    shopkeeper_script->notify(ShopkeeperAssaultEvent{});
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
            .quantity(2)
            .finalize();

    auto& scripting_component = registry.get<ScriptingComponent>(_shopkeeper);
    auto* shopkeeper_script = scripting_component.get<prefabs::ShopkeeperScript>();
    shopkeeper_script->notify(ShopkeeperAssaultEvent{});
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
            .quantity(2)
            .finalize();

    auto& scripting_component = registry.get<ScriptingComponent>(_shopkeeper);
    auto* shopkeeper_script = scripting_component.get<prefabs::ShopkeeperScript>();
    shopkeeper_script->notify(ShopkeeperAssaultEvent{});
    shopkeeper_script->get_angry(_shopkeeper);
    shopkeeper_script->enter_state(&shopkeeper_script->_states.dead, _shopkeeper);
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

    ItemComponent item(ItemType::BODY, ItemApplication::THROWABLE, ItemSlot::ACTIVE);
    item.set_weight(7);
    registry.emplace<ItemComponent>(shopkeeper, item);

    _angry = true;
}

void prefabs::ShopkeeperScript::do_angry_stuff(entt::entity shopkeeper, uint32_t delta_t_ms)
{
    if (!_angry)
    {
        return;
    }

    auto& registry = EntityRegistry::instance().get_registry();
    auto& position = registry.get<PositionComponent>(shopkeeper);
    auto& physics = registry.get<PhysicsComponent>(shopkeeper);

    auto& item_carrier = registry.get<ItemCarrierComponent>(shopkeeper);
    if (item_carrier.has_active_item())
    {
        auto item_entity = item_carrier.get_item(ItemType::SHOTGUN);
        if (item_entity != entt::null)
        {
            auto &activable = registry.get<ActivableComponent>(item_entity);
            activable.activated = true;
        }
    }

    auto& horizontal_orientation_component = registry.get<HorizontalOrientationComponent>(shopkeeper);
    switch (horizontal_orientation_component.orientation)
    {
        case HorizontalOrientation::LEFT: physics.set_x_velocity(-0.11f); break;
        case HorizontalOrientation::RIGHT: physics.set_x_velocity(0.11f); break;
        default: assert(false);
    }

    if ((physics.is_left_collision() || physics.is_right_collision()) && physics.is_bottom_collision())
    {
        physics.set_y_velocity(-1.0f * static_cast<float>(std::rand() % 240) / 1000.0f);
    }

    _horizontal_orientation_switch_timer_ms -= delta_t_ms;

    if (_horizontal_orientation_switch_timer_ms <= 0 && physics.is_bottom_collision())
    {
        if (physics.is_left_collision())
        {
            horizontal_orientation_component.orientation = HorizontalOrientation::RIGHT;
        }
        else if (physics.is_right_collision())
        {
            horizontal_orientation_component.orientation = HorizontalOrientation::LEFT;
        }

        _horizontal_orientation_switch_timer_ms = 1000 + std::rand() % 1000;
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
                physics.set_x_velocity(copysign(0.05f, -pos_diff_x));
            }
        }
    });
}
