#include "prefabs/items/Cape.hpp"

#include "components/generic/ItemComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/ActivableComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

namespace
{
    class CapeScript final : public ScriptBase
    {
    public:
        // TODO: Show icon when in inventory (but only in main-dude's inventory, not when i.e some other thing carries it)
        // ^ HUD system for tracking main dude inventory? Sounds better than InventoryEntity
        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& item = registry.get<ItemComponent>(owner);

            if (!item.is_carried())
            {
                return;
            }
            else if (!_frame_changed)
            {
                auto& quad = registry.get<QuadComponent>(owner);
                quad.frame_changed(CollectiblesSpritesheetFrames::CAPE_LEFT);
                _frame_changed = true;
            }

            auto& carrier_physics = registry.get<PhysicsComponent>(item.get_item_carrier_entity());
            auto& activable = registry.get<ActivableComponent>(owner);
            auto& position = registry.get<PositionComponent>(owner);
            auto& animation = registry.get<AnimationComponent>(owner);
            auto& quad = registry.get<QuadComponent>(owner);

            if (carrier_physics.is_bottom_collision())
            {
                _activated = false;
            }
            else if (activable.activated)
            {
                _activated = !_activated;
            }

            if (!_activated)
            {
                animation.stop();
                quad.frame_changed(CollectiblesSpritesheetFrames::CAPE_LEFT);
            }
            else
            {
                carrier_physics.set_y_velocity(std::min(carrier_physics.get_y_velocity(), 0.070f));

                if (animation.is_finished())
                {
                    quad.frame_changed(CollectiblesSpritesheetFrames::CAPE_ACTIVATED_0_FIRST);
                    animation.start(static_cast<std::size_t>(CollectiblesSpritesheetFrames::CAPE_ACTIVATED_0_FIRST),
                                    static_cast<std::size_t>(CollectiblesSpritesheetFrames::CAPE_ACTIVATED_4_LAST),
                                    100, true);
                }
                else
                {
                    animation.resume();
                }
            }
        }
    private:
        bool _activated = false;
        bool _frame_changed = false;
    };
}

entt::entity prefabs::Cape::create()
{
    return create(0, 0);
}

entt::entity prefabs::Cape::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 1.0f;

    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    quad.frame_changed(CollectiblesSpritesheetFrames::CAPE);

    PhysicsComponent physics(width, height);
    physics.set_friction(0.02f);

    ActivableComponent activable;
    activable.activate_combination = { InputEvent::JUMPING_PRESSED };

    ItemComponent item(ItemType::CAPE, ItemApplication::ACTIVABLE, ItemSlot::BACK);
    item.set_carrying_offset({-0.4f, 0.0f});
    item.set_weight(7);

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_5_PROPS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<ItemComponent>(entity, item);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<CapeScript>());
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<ActivableComponent>(entity, activable);
    registry.emplace<AnimationComponent>(entity);

    return entity;
}
