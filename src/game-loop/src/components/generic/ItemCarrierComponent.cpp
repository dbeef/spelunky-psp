#include "components/generic/ItemCarrierComponent.hpp"
#include "components/generic/SaleableComponent.hpp"
#include "components/damage/GiveProjectileDamageComponent.hpp"

#include <cassert>
#include <cmath>
#include <algorithm>

ItemComponent &ItemCarrierComponent::get_active_item() const
{
    assert(has_active_item());
    auto& registry = EntityRegistry::instance().get_registry();
    return registry.get<ItemComponent>(_slots.active_item);
}

bool ItemCarrierComponent::has_active_item() const
{
    return _slots.active_item != entt::null;
}

bool ItemCarrierComponent::has_passive_item_feet() const
{
    return _slots.passive_item_feet != entt::null;
}

bool ItemCarrierComponent::has_passive_item_hands() const
{
    return _slots.passive_item_hands != entt::null;
}

bool ItemCarrierComponent::has_passive_item_back() const
{
    return _slots.passive_item_back != entt::null;
}

void ItemCarrierComponent::pick_up_item(entt::entity item, entt::entity carrier)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& item_component = registry.get<ItemComponent>(item);

    item_component.set_carrier(carrier);

    switch (item_component.get_slot())
    {
        case ItemSlot::ACTIVE:
        {
            assert(!has_active_item());
            _slots.active_item = item;
            break;
        }
        case ItemSlot::BACK: place_passive_item(_slots.passive_item_back, item); break;
        case ItemSlot::HANDS: place_passive_item(_slots.passive_item_hands, item); break;
        case ItemSlot::FEET: place_passive_item(_slots.passive_item_feet, item); break;
        case ItemSlot::OTHER: _slots.other.push_back(item); break;
        default: assert(false);
    }

    if (registry.has<PhysicsComponent>(item))
    {
        auto &physics = registry.get<PhysicsComponent>(item);
        physics.disable_gravity();
    }

    notify({item, ItemCarrierEvent::EventType::ADDED, item_component.get_type()});
    recalculate_modifiers();
}

void ItemCarrierComponent::put_down_active_item()
{
    if (!has_active_item())
    {
        return;
    }

    auto& registry = EntityRegistry::instance().get_registry();

    auto& item = registry.get<ItemComponent>(_slots.active_item);
    auto& physics = registry.get<PhysicsComponent>(_slots.active_item);

    if (registry.has<GiveProjectileDamageComponent>(_slots.active_item))
    {
        auto& projectile_damage_component = registry.get<GiveProjectileDamageComponent>(_slots.active_item);
        projectile_damage_component.set_last_throw_source(item.get_item_carrier_entity());
    }

    item.reset_carrier();
    physics.enable_gravity();

    notify({_slots.active_item, ItemCarrierEvent::EventType::REMOVED, item.get_type()});
    _slots.active_item = entt::null;
}

void ItemCarrierComponent::throw_active_item(HorizontalOrientationComponent carrier_orientation, PhysicsComponent &carrier_physics)
{
    // TODO: Checking if there's a tile over the throwing component, if so, then throw over more horizontal trajectory.
    // TODO: Throwing upwards when up-key pressed.

    assert(has_active_item());
    auto& registry = EntityRegistry::instance().get_registry();

    auto& item = registry.get<ItemComponent>(_slots.active_item);
    auto& physics = registry.get<PhysicsComponent>(_slots.active_item);

    if (registry.has<GiveProjectileDamageComponent>(_slots.active_item))
    {
        auto& projectile_damage_component = registry.get<GiveProjectileDamageComponent>(_slots.active_item);
        projectile_damage_component.set_last_throw_source(item.get_item_carrier_entity());
    }

    item.reset_carrier();
    physics.enable_gravity();
    physics.set_y_velocity(-0.085);

    const float base_x_velocity = 0.5f + _modifiers.additional_throw_x_velocity;
    const float velocity = (base_x_velocity / item.get_weight_kilos()) + std::fabs(carrier_physics.get_x_velocity());

    switch (carrier_orientation.orientation)
    {
        case HorizontalOrientation::LEFT: physics.set_x_velocity(-velocity); break;
        case HorizontalOrientation::RIGHT: physics.set_x_velocity(velocity); break;
        default: assert(false);
    }

    notify({_slots.active_item, ItemCarrierEvent::EventType::REMOVED, item.get_type()});
    _slots.active_item = entt::null;
}

void ItemCarrierComponent::update_carried_items_positions(PositionComponent &position)
{
    if (has_active_item()) set_item_position(position, _slots.active_item);
    if (has_passive_item_feet()) set_item_position(position, _slots.passive_item_feet);
    if (has_passive_item_back()) set_item_position(position, _slots.passive_item_back);
    if (has_passive_item_hands()) set_item_position(position, _slots.passive_item_hands);

    auto& registry = EntityRegistry::instance().get_registry();
    for (const auto& item : _slots.other)
    {
        set_item_position(position, item);
    }
}

void ItemCarrierComponent::update_carried_items_orientation(HorizontalOrientationComponent &orientation)
{
    if (has_active_item()) set_item_orientation(orientation, _slots.active_item);
    if (has_passive_item_feet()) set_item_orientation(orientation, _slots.passive_item_feet);
    if (has_passive_item_back()) set_item_orientation(orientation, _slots.passive_item_back);
    if (has_passive_item_hands()) set_item_orientation(orientation, _slots.passive_item_hands);

    auto& registry = EntityRegistry::instance().get_registry();
    for (const auto& item : _slots.other)
    {
        set_item_orientation(orientation, item);
    }
}

void ItemCarrierComponent::set_item_position(PositionComponent &position, entt::entity item)
{
    auto& registry = EntityRegistry::instance().get_registry();
    if (!registry.has<PositionComponent>(item))
    {
        return;
    }

    auto& item_position = registry.get<PositionComponent>(item);

    float x_offset = 0.0f;

    auto& physics = registry.get<PhysicsComponent>(item);
    auto& item_component = registry.get<ItemComponent>(item);
    const auto& carrying_offset = item_component.get_carrying_offset();

    if (registry.has<HorizontalOrientationComponent>(item))
    {
        auto &orientation = registry.get<HorizontalOrientationComponent>(item);

        switch (orientation.orientation)
        {
            case HorizontalOrientation::LEFT:
            {
                x_offset -= carrying_offset.x + physics.get_width() * 0.25f;
                break;
            }
            case HorizontalOrientation::RIGHT:
            {
                x_offset += carrying_offset.x + physics.get_width() * 0.25f;
                break;
            }
            default: assert(false);
        }
    }

    item_position.x_center = position.x_center + x_offset;
    item_position.y_center = position.y_center + carrying_offset.y;
}

void ItemCarrierComponent::set_item_orientation(HorizontalOrientationComponent &orientation, entt::entity item)
{
    auto& registry = EntityRegistry::instance().get_registry();
    if (!registry.has<HorizontalOrientationComponent>(item))
    {
        return;
    }

    auto& item_orientation = registry.get<HorizontalOrientationComponent>(item);
    item_orientation.orientation = orientation.orientation;
}

bool ItemCarrierComponent::is_carried(entt::entity item_entity) const
{
    return item_entity == _slots.passive_item_back ||
           item_entity == _slots.passive_item_feet ||
           item_entity == _slots.passive_item_hands ||
           item_entity == _slots.active_item ||
           std::find(_slots.other.begin(), _slots.other.end(), item_entity) != _slots.other.end();
}

std::vector<ItemType> ItemCarrierComponent::get_items() const
{
    auto& registry = EntityRegistry::instance().get_registry();

    std::vector<ItemType> out;
    std::vector<entt::entity> item_entities = get_all_carried_items();

    for (const auto& entity : item_entities)
    {
        if (entity == entt::null)
        {
            continue;
        }

        auto &item_component = registry.get<ItemComponent>(entity);
        out.push_back(item_component.get_type());
    }

    return out;
}

std::vector<ItemType> ItemCarrierComponent::get_non_saleable_items() const
{
    auto& registry = EntityRegistry::instance().get_registry();

    std::vector<ItemType> out;
    std::vector<entt::entity> item_entities = get_all_carried_items();

    for (const auto& entity : item_entities)
    {
        if (entity == entt::null)
        {
            continue;
        }

        if (registry.has<SaleableComponent>(entity))
        {
            continue;
        }

        auto &item_component = registry.get<ItemComponent>(entity);
        out.push_back(item_component.get_type());
    }

    return out;
}

std::vector<entt::entity> ItemCarrierComponent::get_all_carried_items() const
{
    std::vector<entt::entity> out =
            {
                    _slots.active_item,
                    _slots.passive_item_feet,
                    _slots.passive_item_hands,
                    _slots.passive_item_back
            };
    std::copy(_slots.other.begin(), _slots.other.end(), std::back_inserter(out));
    return out;
}

entt::entity ItemCarrierComponent::get_item(ItemType item_type) const
{
    auto& registry = EntityRegistry::instance().get_registry();
    std::vector<entt::entity> item_entities = get_all_carried_items();

    for (const auto& entity : item_entities)
    {
        if (entity == entt::null)
        {
            continue;
        }

        auto &item_component = registry.get<ItemComponent>(entity);
        if (item_component.get_type() == item_type)
        {
            return entity;
        }
    }

    return entt::null;
}
void ItemCarrierComponent::place_passive_item(entt::entity &slot, entt::entity item)
{
    auto& registry = EntityRegistry::instance().get_registry();

    if (slot != entt::null)
    {
        auto& item_component = registry.get<ItemComponent>(slot);
        notify({item, ItemCarrierEvent::EventType::REMOVED, item_component.get_type()});
        registry.destroy(slot);
    }

    slot = item;
}

void ItemCarrierComponent::recalculate_modifiers()
{
    const auto items = get_non_saleable_items();

    _modifiers.can_climb_vertical_surfaces = has_item(ItemType::GLOVE, items);
    _modifiers.additional_jump_on_top_damage = has_item(ItemType::SPIKE_SHOES, items) ? 1 : 0;
    _modifiers.additional_throw_x_velocity = has_item(ItemType::MITT, items) ? 0.2f : 0.0f;
    _modifiers.additional_jump_y_velocity = has_item(ItemType::SPRING_SHOES, items) ? 0.05f : 0.0f;
}

bool ItemCarrierComponent::has_item(ItemType type, const std::vector<ItemType>& items) const
{
    return std::any_of(items.begin(), items.end(), [type](const auto &item_type)
    {
        return type == item_type;
    });
}

std::vector<entt::entity> ItemCarrierComponent::get_item_entities() const
{
    std::vector<entt::entity> item_entities =
            {
                    _slots.active_item,
                    _slots.passive_item_feet,
                    _slots.passive_item_hands,
                    _slots.passive_item_back
            };
    std::copy(_slots.other.begin(), _slots.other.end(), std::back_inserter(item_entities));
    return item_entities;
}
