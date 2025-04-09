#include "prefabs/items/Compass.hpp"

#include "components/damage/GiveProjectileDamageComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

entt::entity prefabs::Compass::create()
{
    return create(0, 0);
}

entt::entity prefabs::Compass::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 0.75f;

    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    quad.frame_changed(CollectiblesSpritesheetFrames::COMPASS);

    ItemComponent item(ItemType::COMPASS, ItemApplication::PASSIVE, ItemSlot::OTHER);
    item.set_weight(7);

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_3_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, width, height);
    registry.emplace<ItemComponent>(entity, item);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<CompassScript>());
    registry.emplace<HorizontalOrientationComponent>(entity);

    return entity;
}

void prefabs::CompassScript::update(entt::entity owner, uint32_t delta_time_ms)
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

void prefabs::CompassScript::update_arrow(prefabs::CompassArrow arrow)
{
    if (arrow != _last_issued_arrow)
    {
        notify(arrow);
    }

    _last_issued_arrow = arrow;
}
