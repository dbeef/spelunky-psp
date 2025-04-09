#include "prefabs/items/Shotgun.hpp"
#include "prefabs/other/Bullet.hpp"
#include "prefabs/effects/ShotgunBlast.hpp"

#include "components/generic/ItemComponent.hpp"
#include "components/generic/ActivableComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"

#include "audio/Audio.hpp"
#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

namespace
{
    constexpr uint16_t cooldown_time_ms =  750;

    class ShotgunScript final : public ScriptBase
    {
    public:
        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& item = registry.get<ItemComponent>(owner);

            if (!item.is_carried())
            {
                return;
            }

            _cooldown_timer_ms += delta_time_ms;

            auto& activable = registry.get<ActivableComponent>(owner);
            auto& position = registry.get<PositionComponent>(owner);
            auto& physics = registry.get<PhysicsComponent>(owner);
            auto& orientation = registry.get<HorizontalOrientationComponent>(owner);

            if (activable.activated && _cooldown_timer_ms >= cooldown_time_ms)
            {
                Audio::instance().play(SFXType::SHOTGUN);

                _cooldown_timer_ms = 0;

                float bullet_velocity_x = 0;
                float bullet_velocity_y = 0;
                float bullet_x_pos_offset = 0;

                switch (orientation.orientation)
                {
                    case HorizontalOrientation::LEFT: bullet_velocity_x = -0.3f; bullet_x_pos_offset = -physics.get_width() * 0.8f; break;
                    case HorizontalOrientation::RIGHT: bullet_velocity_x = 0.3f; bullet_x_pos_offset = physics.get_width() * 0.8f; break;
                }

                {
                    auto bullet = prefabs::Bullet::create(position.x_center + (bullet_x_pos_offset * 0.33f), position.y_center, item.get_item_carrier_entity());
                    auto& bullet_physics = registry.get<PhysicsComponent>(bullet);
                    bullet_physics.set_velocity(bullet_velocity_x, bullet_velocity_y);
                }

                {
                    auto bullet = prefabs::Bullet::create(position.x_center + (bullet_x_pos_offset * 0.66f), position.y_center, item.get_item_carrier_entity());
                    auto& bullet_physics = registry.get<PhysicsComponent>(bullet);
                    bullet_physics.set_velocity(bullet_velocity_x, bullet_velocity_y + 0.015f);
                }

                {
                    auto bullet = prefabs::Bullet::create(position.x_center + (bullet_x_pos_offset * 1.00f), position.y_center, item.get_item_carrier_entity());
                    auto& bullet_physics = registry.get<PhysicsComponent>(bullet);
                    bullet_physics.set_velocity(bullet_velocity_x, bullet_velocity_y - 0.015f);
                }

                auto blast = prefabs::ShotgunBlast::create(position.x_center + bullet_x_pos_offset, position.y_center - (physics.get_height() * 0.15f));
                auto& blast_orientation = registry.get<HorizontalOrientationComponent>(blast);
                blast_orientation.orientation = orientation.orientation;
            }
        }
    private:
        uint32_t _cooldown_timer_ms = cooldown_time_ms;
    };
}

entt::entity prefabs::Shotgun::create()
{
    return create(0, 0);
}

entt::entity prefabs::Shotgun::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 0.5f;

    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    quad.frame_changed(CollectiblesSpritesheetFrames::SHOTGUN);

    PhysicsComponent physics(width, height);
    physics.set_friction(0.02f);

    ActivableComponent activable;
    activable.activate_combination = { InputEvent::THROWING_PRESSED };

    ItemComponent item(ItemType::SHOTGUN, ItemApplication::ACTIVABLE, ItemSlot::ACTIVE);
    item.set_carrying_offset({0.1f, 0.1f});
    item.set_weight(7);

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_3_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<ItemComponent>(entity, item);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<ShotgunScript>());
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<ActivableComponent>(entity, activable);

    return entity;
}
