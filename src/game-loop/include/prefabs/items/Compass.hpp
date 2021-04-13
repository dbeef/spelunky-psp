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
        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            constexpr float HORIZONTAL_THRESHOLD = 5.0f;
            constexpr float VERTICAL_THRESHOLD = 5.0f;

            auto& registry = EntityRegistry::instance().get_registry();
            auto& item_component = registry.get<ItemComponent>(owner);

            if (item_component.is_carried())
            {
                if (!_hidden)
                {
                    registry.remove<QuadComponent>(owner);
                    registry.remove<MeshComponent>(owner);
                    _hidden = true;
                }

                // Check exit-door position relative to *item carrier* position and issue CompassPositionEvent if changed:

                MapTile* exit_tile = nullptr;
                Level::instance().get_tile_batch().get_first_tile_of_given_type(MapTileType::EXIT, exit_tile);
                assert(exit_tile);

                const auto item_carrier_entity = item_component.get_item_carrier_entity();
                const auto& item_carrier_position = registry.get<PositionComponent>(item_carrier_entity);

                const auto pos_x_diff = item_carrier_position.x_center - exit_tile->get_center_x();
                const auto pos_y_diff = item_carrier_position.y_center - exit_tile->get_center_y();

                if (std::fabs(pos_x_diff) <= HORIZONTAL_THRESHOLD && std::fabs(pos_y_diff) <= VERTICAL_THRESHOLD)
                {
                    update_arrow(CompassArrow::NONE);
                    return;
                }

                if (std::fabs(pos_y_diff) <= VERTICAL_THRESHOLD)
                {
                    // Item carrier is roughly on the same level as exit door, issue left/right arrows:
                    if (pos_x_diff > 0.0f)
                    {
                        update_arrow(CompassArrow::LEFT);
                    }
                    else
                    {
                        update_arrow(CompassArrow::RIGHT);
                    }
                }
                else
                {
                    // Item carrier is above the exit door, issue down/left-bottom/right-bottom arrows:
                    if (std::fabs(pos_x_diff) <= HORIZONTAL_THRESHOLD)
                    {
                        update_arrow(CompassArrow::DOWN);
                    }
                    else if (pos_x_diff > 0)
                    {
                        update_arrow(CompassArrow::LEFT_BOTTOM);
                    }
                    else
                    {
                        update_arrow(CompassArrow::RIGHT_BOTTOM);
                    }
                }
            }
        }
    private:
        void update_arrow(CompassArrow arrow)
        {
            if (arrow != _last_issued_arrow)
            {
                notify(arrow);
            }

            _last_issued_arrow = arrow;
        }

        CompassArrow _last_issued_arrow = CompassArrow::NONE;
        bool _hidden = false;
    };

    struct Compass
    {
        static entt::entity create(float pos_x_center, float pos_y_center);
        static entt::entity create();
    };
}

