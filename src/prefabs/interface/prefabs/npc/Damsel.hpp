#pragma once

#include "entt/entt.hpp"

namespace prefabs
{
    struct Damsel
    {
        // FIXME: damsel_rescued should be an event!
        static entt::entity create(bool& damsel_rescued, float pos_x_center, float pos_y_center);
        static entt::entity create(bool& damsel_rescued);
    };
}
