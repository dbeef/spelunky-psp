#pragma once

#include "other/ItemType.hpp"
#include "entt/entt.hpp"
#include "Point2D.hpp"

enum class ItemApplication
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
    ItemComponent(ItemType type, ItemApplication application, ItemSlot slot)
        : _type(type)
        , _application(application)
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

    ItemType get_type() const { return _type; }
    ItemSlot get_slot() const { return _slot; }
    ItemApplication get_application() const { return _application; }

    void set_slot(ItemSlot slot) { _slot = slot; }
    void set_type(ItemApplication application) { _application = application; } // FIXME: More like set_application

private:

    ItemType _type{};
    ItemApplication _application{};
    ItemSlot _slot{};
    entt::entity _carrier = entt::null;
    Point2D _carrying_offset = {0.0f, 0.0f};

    // For throwing:
    float _weight_kilos = 1.0f;
};
