#pragma once

#include "entt/entt.hpp"

namespace prefabs
{
    struct Damsel
    {
        static entt::entity create(bool& damsel_rescued, float pos_x_center, float pos_y_center);
        static entt::entity create(bool& damsel_rescued);
    };
}
