#include "prefabs/items/Rope.hpp"
#include "prefabs/items/RopeChainElement.hpp"

#include "components/damage/GiveProjectileDamageComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/ActivableComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"

#include "EntityRegistry.hpp"
#include "Level.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

#include <cmath>

namespace
{
    const float max_distance_up = MapTile::PHYSICAL_HEIGHT * 8.0f;

    class RopeScript final : public ScriptBase
    {
    public:

        explicit RopeScript(Point2D throw_origin) : _throw_origin(throw_origin)
        {
        }

        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& item = registry.get<ItemComponent>(owner);
            auto& position = registry.get<PositionComponent>(owner);

            if (!_thrown)
            {
                auto &activable = registry.get<ActivableComponent>(owner);
                if (activable.activated)
                {
                    auto &carrier = item.get_item_carrier();
                    carrier.put_down_active_item();
                    registry.remove<ActivableComponent>(owner);

                    position.x_center = std::floor(position.x_center) + MapTile::PHYSICAL_WIDTH / 2;
                    position.y_center = std::floor(position.y_center) + MapTile::PHYSICAL_HEIGHT / 2;

                    _thrown = true;
                }
            }
            else if (!_hit_the_ceiling)
            {
                auto& physics = registry.get<PhysicsComponent>(owner);

                if (physics.is_upper_collision() || std::fabs(_throw_origin.y - position.y_center) >= max_distance_up)
                {
                    auto& quad = registry.get<QuadComponent>(owner);
                    quad.frame_changed(CollectiblesSpritesheetFrames::ROPE_ENDING);
                    registry.remove<PhysicsComponent>(owner);
                    _hit_the_ceiling = true;
                }
                else
                {
                    physics.set_y_velocity(-0.25f);
                    physics.set_x_velocity(0);
                }
            }
            else if (!_dispatched_rope_chain)
            {
                _spawning_timer_ms += delta_time_ms;
                if (_spawning_timer_ms > 30)
                {
                    float element_x_center = position.x_center;
                    float element_y_center = position.y_center;

                    element_y_center += 0.5f * MapTile::PHYSICAL_HEIGHT * (_spawned_rope_chain_elements + 1);

                    auto* overlapped_tile = Level::instance().get_tile_batch().map_tiles[static_cast<int>(element_x_center)][static_cast<int>(element_y_center)];

                    if (overlapped_tile->collidable)
                    {
                        _dispatched_rope_chain = true;
                        return;
                    }

                    if (_spawned_rope_chain_elements != 0)
                    {
                        Point2D new_chain_position(element_x_center, element_y_center);
                        if (new_chain_position == _last_spawned_chain)
                        {
                            return;
                        }
                    }

                    _spawned_rope_chain_elements++;

                    if (_spawned_rope_chain_elements >= 16)
                    {
                        _dispatched_rope_chain = true;
                    }

                    prefabs::RopeChainElement::create(element_x_center, element_y_center);
                    overlapped_tile->climbable = true;
                    _spawning_timer_ms = 0;
                    _last_spawned_chain = Point2D(element_x_center, element_y_center);
                }
            }
        }
    private:
        Point2D _throw_origin;
        Point2D _last_spawned_chain;

        uint8_t _spawned_rope_chain_elements = 0;
        uint32_t _spawning_timer_ms = 0;

        bool _dispatched_rope_chain = false;
        bool _thrown = false;
        bool _hit_the_ceiling = false;
    };
}

entt::entity prefabs::Rope::create()
{
    return create(0, 0);
}

entt::entity prefabs::Rope::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 0.5f;
    const float height = 0.5f;

    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    quad.frame_changed(CollectiblesSpritesheetFrames::ROPE_ITEM);

    ItemComponent item(ItemType::ROPE, ItemApplication::ACTIVABLE, ItemSlot::ACTIVE);
    item.set_weight(2);
    item.set_carrying_offset({0.1f, 0.0f});

    ActivableComponent activable;
    activable.activate_combination = { InputEvent::THROWING_PRESSED };

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_3_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, width, height);
    registry.emplace<ItemComponent>(entity, item);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<RopeScript>(Point2D(pos_x_center, pos_y_center)));
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<ActivableComponent>(entity, activable);
    registry.emplace<GiveProjectileDamageComponent>(entity, 1);

    return entity;
}
