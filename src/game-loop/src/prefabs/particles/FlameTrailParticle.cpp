#include "prefabs/particles/FlameTrailParticle.hpp"

#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/TimeLimitComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

entt::entity prefabs::FlameTrailParticle::create()
{
    return create(0, 0);
}

entt::entity prefabs::FlameTrailParticle::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 0.5f;
    const float height = 0.5f;

    PositionComponent position(pos_x_center, pos_y_center);
    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    MeshComponent mesh;
    PhysicsComponent physics(width, height);
    AnimationComponent animation;

    physics.set_gravity_modifier(0.35f);
    physics.set_bounciness(0.4f);
    physics.set_friction(0.1f);

    quad.frame_changed<CollectiblesSpritesheetFrames>(CollectiblesSpritesheetFrames::FLAME_TRAIL_0_FIRST);

    animation.start(static_cast<std::size_t>(CollectiblesSpritesheetFrames::FLAME_TRAIL_0_FIRST),
                    static_cast<std::size_t>(CollectiblesSpritesheetFrames::FLAME_TRAIL_4_LAST),
                    110, false);

    mesh.rendering_layer = RenderingLayer::LAYER_3_DUDE;
    mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

    registry.emplace<PositionComponent>(entity, position);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);
    registry.emplace<AnimationComponent>(entity, animation);
    registry.emplace<TimeLimitComponent>(entity, 550);
//    registry.emplace<PhysicsComponent>(entity, physics);

    return entity;
}
