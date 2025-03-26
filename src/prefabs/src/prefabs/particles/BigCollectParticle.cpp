#include "prefabs/particles/BigCollectParticle.hpp"

#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/TimeLimitComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

entt::entity prefabs::BigCollectParticle::create()
{
    return create(0, 0);
}

entt::entity prefabs::BigCollectParticle::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 1.0f;

    PositionComponent position(pos_x_center, pos_y_center);
    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    MeshComponent mesh;
    PhysicsComponent physics(0.01f, 0.01f);
    physics.disable_gravity();
    physics.set_y_velocity(-0.001f);

    quad.frame_changed<CollectiblesSpritesheetFrames>(CollectiblesSpritesheetFrames::BIG_COLLECT_EFFECT);
    mesh.rendering_layer = RenderingLayer::LAYER_3_ITEMS;
    mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

    registry.emplace<PositionComponent>(entity, position);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<TimeLimitComponent>(entity, 2000);

    return entity;
}
