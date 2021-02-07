#include "prefabs/other/Bullet.hpp"
#include "prefabs/particles/SmokePuffParticle.hpp"

#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

namespace
{
    class BulletScript final : public ScriptBase
    {
    public:
        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& physics = registry.get<PhysicsComponent>(owner);

            if (physics.is_left_collision() || physics.is_right_collision() ||
                physics.is_upper_collision() || physics.is_bottom_collision())
            {
                auto& position = registry.get<PositionComponent>(owner);
                prefabs::SmokePuffParticle::create(position.x_center, position.y_center);
                registry.destroy(owner);
            }
        }
    };
}

entt::entity prefabs::Bullet::create()
{
    return create(0, 0);
}

entt::entity prefabs::Bullet::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 0.5f;
    const float height = 0.5f;

    PositionComponent position(pos_x_center, pos_y_center);
    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    MeshComponent mesh;
    PhysicsComponent physics(width, height);

    physics.disable_gravity();

    quad.frame_changed(CollectiblesSpritesheetFrames::BULLET);
    mesh.rendering_layer = RenderingLayer::LAYER_2_ITEMS;
    mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

    registry.emplace<PositionComponent>(entity, position);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<BulletScript>());

    return entity;
}
