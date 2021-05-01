#pragma once

#include "other/ItemType.hpp"

#include <entt/entt.hpp>
#include <unordered_map>
#include <vector>

namespace populator
{
    class Shop
    {
    public:
        Shop();
        entt::entity make_next_item(float x_center, float y_center);
    private:
        std::size_t _items_made = 0;
        std::vector<ItemType> _items;
    };
}
