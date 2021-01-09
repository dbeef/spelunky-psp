#pragma once

#include "entt/entt.hpp"
#include "Point2D.hpp"

enum class ItemType
{
    THROWABLE,
    ACTIVABLE,
    OPENABLE,
    PASSIVE
};

enum class ItemSlot
{
    ACTIVE,
    FEET,
    HANDS,
    BACK,
    OTHER
};

class ItemCarrierComponent;
class ItemComponent
{
public:

    ItemComponent() = default;
    ItemComponent(ItemType type, ItemSlot slot)
        : _type(type)
        , _slot(slot)
    {}

    bool is_carried() const { return _carrier != entt::null; }
    ItemCarrierComponent& get_item_carrier() const;
    entt::entity get_item_carrier_entity() const { return _carrier; }
    void set_carrier(entt::entity carrier) { _carrier = carrier; }
    void reset_carrier() { _carrier = entt::null; }

    void set_carrying_offset(const Point2D& carrying_offset) { _carrying_offset = carrying_offset; }
    const Point2D& get_carrying_offset() const { return _carrying_offset; }

    void set_weight(float weight_kilos) { _weight_kilos = weight_kilos; };
    float get_weight_kilos() const { return _weight_kilos; };

    ItemSlot get_slot() const { return _slot; }
    ItemType get_type() const { return _type; }

    void set_slot(ItemSlot slot) { _slot = slot; }
    void set_type(ItemType type) { _type = type; }

private:

    ItemType _type{};
    ItemSlot _slot{};
    entt::entity _carrier = entt::null;
    Point2D _carrying_offset = {0.0f, 0.0f};

    // For throwing:
    float _weight_kilos = 1.0f;
};
