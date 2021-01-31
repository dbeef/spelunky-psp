#include "components/generic/ItemCarrierComponent.hpp"

#include <cassert>
#include <cmath>

namespace
{
    void place_passive_item(entt::entity& slot, entt::entity item)
    {
        auto& registry = EntityRegistry::instance().get_registry();

        if (slot != entt::null)
        {
            registry.destroy(slot);
        }

        slot = item;
    }
}

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
}

void ItemCarrierComponent::put_down_active_item()
{
    assert(has_active_item());
    auto& registry = EntityRegistry::instance().get_registry();

    auto& item = registry.get<ItemComponent>(_slots.active_item);
    auto& physics = registry.get<PhysicsComponent>(_slots.active_item);

    item.reset_carrier();
    physics.enable_gravity();

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

    item.reset_carrier();
    physics.enable_gravity();
    physics.set_y_velocity(-0.085);

    const float base_x_velocity = 0.5f;
    const float velocity = (base_x_velocity / item.get_weight_kilos()) + std::fabs(carrier_physics.get_x_velocity());

    switch (carrier_orientation.orientation)
    {
        case HorizontalOrientation::LEFT: physics.set_x_velocity(-velocity); break;
        case HorizontalOrientation::RIGHT: physics.set_x_velocity(velocity); break;
        default: assert(false);
    }

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
