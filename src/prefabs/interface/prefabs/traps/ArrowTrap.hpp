#pragma once

#include "components/generic/HorizontalOrientationComponent.hpp"
#include "entt/entt.hpp"

namespace prefabs
{
    struct ArrowTrap
    {
        static entt::entity create(float pos_x_center, float pos_y_center, HorizontalOrientation);
    };
}
