#pragma once

#include "components/generic/ItemComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "other/ItemType.hpp"
#include "EntityRegistry.hpp"
#include "entt/entt.hpp"
#include "patterns/Subject.hpp"

#include <vector>
#include <algorithm>

struct ItemCarrierEvent
{
    enum class EventType
    {
        ADDED,
        REMOVED
    };

    EventType event_type;
    ItemType item_type;
};

// TODO: ItemCarrierComponent should be a Subject<ItemCarrierComponent>, so Inventory could subscribe to it, and in turn notify HUD
class ItemCarrierComponent : public Subject<ItemCarrierEvent>
{
public:

    bool is_carried(entt::entity) const;
    void pick_up_item(entt::entity item, entt::entity carrier);
    void put_down_active_item();
    void throw_active_item(HorizontalOrientationComponent carrier_orientation, PhysicsComponent& carrier_physics);

    bool has_active_item() const;
    bool has_passive_item_feet() const;
    bool has_passive_item_hands() const;
    bool has_passive_item_back() const;

    ItemComponent& get_active_item() const;

    void update_carried_items_positions(PositionComponent& position);
    void update_carried_items_orientation(HorizontalOrientationComponent& orientation);

    std::vector<ItemType> get_items() const;
    entt::entity get_item(ItemType) const;

private:

    struct
    {
        entt::entity active_item = entt::null;
        entt::entity passive_item_feet = entt::null;
        entt::entity passive_item_hands = entt::null;
        entt::entity passive_item_back = entt::null;
        std::vector<entt::entity> other;
    } _slots;

    void set_item_position(PositionComponent& position, entt::entity item);
    void set_item_orientation(HorizontalOrientationComponent& orientation, entt::entity item);
    std::vector<entt::entity> get_all_carried_items() const;
    void place_passive_item(entt::entity& slot, entt::entity item);
};
