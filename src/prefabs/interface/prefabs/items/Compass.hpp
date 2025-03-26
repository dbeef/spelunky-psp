#pragma once

#include "entt/entt.hpp"
#include "patterns/Subject.hpp"
#include "EntityRegistry.hpp"
#include "Level.hpp"

#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/generic/PositionComponent.hpp"

#include <cmath>

namespace prefabs
{
    enum class CompassArrow
    {
        LEFT,
        RIGHT,
        DOWN,
        LEFT_BOTTOM,
        RIGHT_BOTTOM,
        NONE
    };

    class CompassScript : public ScriptBase, public Subject<CompassArrow>
    {
    public:
        void update(entt::entity owner, uint32_t delta_time_ms) override;
    private:
        void update_arrow(CompassArrow arrow);
        CompassArrow _last_issued_arrow = CompassArrow::NONE;
        bool _hidden = false;
    };

    struct Compass
    {
        static entt::entity create(float pos_x_center, float pos_y_center);
        static entt::entity create();
    };
}

