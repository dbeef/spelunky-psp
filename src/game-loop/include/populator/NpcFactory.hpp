#pragma once

#include "other/NpcType.hpp"
#include <entt/entt.hpp>

class NpcFactory {
public:
    static entt::entity make(NpcType);
    static entt::entity make(NpcType, float pos_x, float pos_y);
};
