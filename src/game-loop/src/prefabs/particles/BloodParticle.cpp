#include "prefabs/particles/BloodParticle.hpp"

#include "components/generic/PositionComponent.hpp"
#include "components/generic/ParticleEmitterComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/TimeLimitComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

entt::entity prefabs::BloodParticle::create()
{
    return create(0, 0);
}

entt::entity prefabs::BloodParticle::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 0.5f;
    const float height = 0.5f;

    PhysicsComponent physics(width, height);
    physics.set_gravity_modifier(0.4f);
    physics.set_bounciness(0.8f);

    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    quad.frame_changed<CollectiblesSpritesheetFrames>(CollectiblesSpritesheetFrames::BLOOD_TRAIL_0_FIRST);

    AnimationComponent animation;
    animation.start(static_cast<std::size_t>(CollectiblesSpritesheetFrames::BLOOD_TRAIL_0_FIRST),
                    static_cast<std::size_t>(CollectiblesSpritesheetFrames::BLOOD_TRAIL_6_LAST),
                    110, true);

    MeshComponent mesh;
    mesh.rendering_layer = RenderingLayer::LAYER_4_DUDE;
    mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

    ParticleEmitterComponent emitter;
    emitter.interval_ms = 250;
    emitter.particle_type = ParticleType::BLOOD_TRAIL;
    emitter.time_since_last_emission_ms = 0;

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<AnimationComponent>(entity, animation);
    registry.emplace<TimeLimitComponent>(entity, 1500);
    registry.emplace<ParticleEmitterComponent>(entity, emitter);

    return entity;
}
