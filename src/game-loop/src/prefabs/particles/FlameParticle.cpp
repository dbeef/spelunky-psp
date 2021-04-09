#include "prefabs/particles/FlameParticle.hpp"
#include "prefabs/particles/SmokePuffParticle.hpp"

#include "components/generic/PositionComponent.hpp"
#include "components/generic/ParticleEmitterComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/MeshComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

namespace
{
    class FlameParticleScript final : public ScriptBase
    {
    public:
        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& physics = registry.get<PhysicsComponent>(owner);

            _time_lived_ms += delta_time_ms;

            if (_time_lived_ms < 1000)
            {
                return;
            }

            if (physics.is_bottom_collision() || physics.is_left_collision() ||
                physics.is_right_collision() || physics.is_upper_collision())
            {
                auto& position = registry.get<PositionComponent>(owner);
                prefabs::SmokePuffParticle::create(position.x_center, position.y_center);
                registry.destroy(owner);
            }
        }
    private:
        uint32_t _time_lived_ms = 0;
    };
}

entt::entity prefabs::FlameParticle::create()
{
    return create(0, 0);
}

entt::entity prefabs::FlameParticle::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 0.5f;
    const float height = 0.5f;

    PositionComponent position(pos_x_center, pos_y_center);
    PhysicsComponent physics(width, height);
    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    MeshComponent mesh;

    ParticleEmitterComponent emitter;
    emitter.interval_ms = 250;
    emitter.particle_type = ParticleType::FLAME_TRAIL;
    emitter.time_since_last_emission_ms = 0;

    physics.set_bounciness(0.8f);
    physics.set_friction(0.0f);
    physics.set_gravity_modifier(0.1f);

    quad.frame_changed<CollectiblesSpritesheetFrames>(CollectiblesSpritesheetFrames::FLAME);

    mesh.rendering_layer = RenderingLayer::LAYER_4_DUDE;
    mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

    registry.emplace<PositionComponent>(entity, position);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<ParticleEmitterComponent>(entity, emitter);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<FlameParticleScript>());

    return entity;
}
