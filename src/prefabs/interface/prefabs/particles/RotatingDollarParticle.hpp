#pragma once

#include "entt/entt.hpp"

namespace prefabs
{
    struct RotatingDollarParticle
    {
        static entt::entity create(float pos_x_center, float pos_y_center);
        static entt::entity create();
        static float get_width();
        static float get_height();
    };
}
