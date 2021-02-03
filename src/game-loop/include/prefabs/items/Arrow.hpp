#pragma once

#include "entt/entt.hpp"

namespace prefabs
{
    struct Arrow
    {
        static entt::entity create(float pos_x_center, float pos_y_center);
        static entt::entity create();
    };
}
