#include "prefabs/particles/PoofParticle.hpp"

#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/TimeLimitComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

entt::entity prefabs::PoofParticle::create()
{
    return create(0, 0);
}

entt::entity prefabs::PoofParticle::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 0.5f;
    const float height = 0.5f;

    PositionComponent position(pos_x_center, pos_y_center);
    AnimationComponent animation;
    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    MeshComponent mesh;
    PhysicsComponent physics(width, height);

    physics.set_gravity_modifier(0.2f);

    animation.start(static_cast<std::size_t>(CollectiblesSpritesheetFrames::POOF_0_FIRST),
                    static_cast<std::size_t>(CollectiblesSpritesheetFrames::POOF_5_LAST),
                    100, false);

    quad.frame_changed<CollectiblesSpritesheetFrames>(CollectiblesSpritesheetFrames::POOF_0_FIRST);

    mesh.rendering_layer = RenderingLayer::LAYER_4_DUDE;
    mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

    registry.emplace<PositionComponent>(entity, position);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<TimeLimitComponent>(entity, 600);
    registry.emplace<AnimationComponent>(entity, animation);

    return entity;
}
