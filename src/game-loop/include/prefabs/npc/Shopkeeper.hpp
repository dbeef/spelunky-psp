#pragma once

#include "entt/entt.hpp"

namespace prefabs
{
    struct Shopkeeper
    {
        static entt::entity create(bool& robbed, float pos_x_center, float pos_y_center);
        static entt::entity create(bool& robbed);
    };
}
