#pragma once

#include <entt/entt.hpp>
#include "LootType.hpp"

class LootFactory {
public:
    static entt::entity make(LootType);
    static entt::entity make(LootType, float pos_x, float pos_y);
};
