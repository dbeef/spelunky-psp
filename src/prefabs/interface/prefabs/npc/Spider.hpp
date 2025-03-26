#pragma once

#include "entt/entt.hpp"

namespace prefabs
{
    struct Spider
    {
        static entt::entity create(float pos_x_center, float pos_y_center, bool triggered = false);
        static entt::entity create();
    };
}
