#include "prefabs/particles/SmoochingParticle.hpp"

#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/TimeLimitComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/NPCSpritesheetFrames.hpp"

entt::entity prefabs::SmoochingParticle::create()
{
    return create(0, 0);
}

entt::entity prefabs::SmoochingParticle::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 1.0f;

    PositionComponent position(pos_x_center, pos_y_center);
    QuadComponent quad(TextureType::NPC, width, height);
    MeshComponent mesh;
    AnimationComponent animation;
    PhysicsComponent physics(0.01f, 0.01f);
    physics.disable_gravity();
    physics.set_y_velocity(-0.001f);

    quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::SMOOCH_HEART_0_START);

    animation.start(static_cast<std::size_t>(NPCSpritesheetFrames::SMOOCH_HEART_0_START),
                    static_cast<std::size_t>(NPCSpritesheetFrames::SMOOCH_HEART_1_LAST),
                    150, true);

    mesh.rendering_layer = RenderingLayer::LAYER_3_ITEMS;
    mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

    registry.emplace<PositionComponent>(entity, position);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<AnimationComponent>(entity, animation);
    registry.emplace<TimeLimitComponent>(entity, 2000);

    return entity;
}
