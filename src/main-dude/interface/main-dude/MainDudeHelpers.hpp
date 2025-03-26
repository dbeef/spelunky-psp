#pragma once

#include <functional>

class PositionComponent;
struct Vector2D;

namespace main_dude_helpers
{
    using WhenDudeIsCloseCallback_t = std::function<void(const PositionComponent& dude_pos, const Vector2D& distance)>;
    void check_main_dude_proximity(const WhenDudeIsCloseCallback_t& callback,
                                   const Vector2D& activation_distance,
                                   const PositionComponent& position
    );
}
