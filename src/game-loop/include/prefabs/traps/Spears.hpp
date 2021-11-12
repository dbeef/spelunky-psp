#pragma once

#include "entt/entt.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"

namespace prefabs
{
    struct Spears
    {
        static entt::entity create(float pos_x_center, float pos_y_center, HorizontalOrientation);
    };
}
