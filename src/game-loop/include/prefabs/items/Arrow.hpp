#pragma once

#include "entt/entt.hpp"

namespace prefabs
{
    struct Arrow
    {
        static entt::entity create(float pos_x_center, float pos_y_center, entt::entity throw_source = entt::null);
        static entt::entity create();
    };
}
