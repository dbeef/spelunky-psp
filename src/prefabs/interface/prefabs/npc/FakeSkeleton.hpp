#pragma once

#include "entt/entt.hpp"

namespace prefabs
{
    struct FakeSkeleton
    {
        static void create(float pos_x_center, float pos_y_center);
        static void create();
    };
}
