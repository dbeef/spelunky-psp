#pragma once

#include "other/ItemType.hpp"
#include <entt/entt.hpp>

class ItemFactory {
public:
    static entt::entity make(ItemType);
    static entt::entity make(ItemType, float pos_x, float pos_y);
};
