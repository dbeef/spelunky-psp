#include "prefabs/items/Skull.hpp"
#include "prefabs/particles/SmokePuffParticle.hpp"
#include "prefabs/particles/BonesParticle.hpp"

#include "components/damage/GiveProjectileDamageComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/NPCSpritesheetFrames.hpp"

#include <cmath>
#include <algorithm>

namespace
{
    class SkullScript final : public ScriptBase
    {
    public:
        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            const auto& physics = registry.get<PhysicsComponent>(owner);

            if (physics.is_left_collision() || physics.is_right_collision() ||
                physics.is_upper_collision() || physics.is_bottom_collision())
            {
                if (std::fabs(_last_x_speed) > 0.15f || std::fabs(_last_y_speed) > 0.13f)
                {
                    const auto& position = registry.get<PositionComponent>(owner);
                    prefabs::SmokePuffParticle::create(position.x_center, position.y_center);
                    registry.destroy(owner);

                    const auto bones_particle = prefabs::BonesParticle::create(position.x_center, position.y_center);
                    auto& bones_particle_physics = registry.get<PhysicsComponent>(bones_particle);

                    _last_x_speed = std::clamp(_last_x_speed, -0.1f, 0.1f);
                    _last_y_speed = std::clamp(_last_y_speed, -0.1f, 0.1f);
                    bones_particle_physics.set_velocity(-_last_x_speed, -_last_y_speed);
                }
            }

            _last_x_speed = physics.get_x_velocity();
            _last_y_speed = physics.get_y_velocity();
        }

    private:
        float _last_x_speed = 0;
        float _last_y_speed = 0;
    };
}

entt::entity prefabs::Skull::create()
{
    return create(0, 0);
}

entt::entity prefabs::Skull::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 0.5f;
    const float height = 0.5f;

    QuadComponent quad(TextureType::NPC, width, height);
    quad.frame_changed(NPCSpritesheetFrames::SKULL);

    PhysicsComponent physics(width, height);
    physics.set_friction(0.02f);

    ItemComponent item(ItemType::THROWABLE, ItemSlot::ACTIVE);
    item.set_weight(2);
    item.set_carrying_offset({0.0f, -1.0f/8.0f});

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<SkullScript>());
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_2_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<ItemComponent>(entity, item);
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<GiveProjectileDamageComponent>(entity, 1);

    return entity;
}
