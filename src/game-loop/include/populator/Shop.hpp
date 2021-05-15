#pragma once

#include "other/ItemType.hpp"
#include "components/generic/ZoneComponent.hpp"
#include "components/generic/PositionComponent.hpp"

#include <entt/entt.hpp>
#include <unordered_map>
#include <vector>

class Shop
{
public:
    Shop(bool robbed);
    entt::entity make_next_item(float x_center, float y_center);
    ZoneComponent get_shop_zone() const { return _shop_zone; };
private:
    bool _robbed = false;
    std::size_t _items_made = 0;
    std::vector<ItemType> _items;
    ZoneComponent _shop_zone = {0.0, 0.0};
    PositionComponent _shop_center = {0.0, 0.0};

    void attach_saleable_component(entt::entity item, int price);
};
