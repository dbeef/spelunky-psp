#pragma once

#include "entt/entt.hpp"

namespace prefabs
{
    struct Bullet
    {
        static entt::entity create(float pos_x_center, float pos_y_center, entt::entity projectile_source = entt::null);
        static entt::entity create();
    };
}
