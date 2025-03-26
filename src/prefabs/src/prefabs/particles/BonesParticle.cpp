#include "prefabs/particles/BonesParticle.hpp"
#include "prefabs/particles/SmokePuffParticle.hpp"

#include "components/generic/PositionComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/NPCSpritesheetFrames.hpp"

namespace
{
    class BonesParticleScript final : public ScriptBase
    {
    public:
        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            const auto& physics = registry.get<PhysicsComponent>(owner);
            if (physics.is_left_collision() || physics.is_right_collision() ||
                physics.is_upper_collision() || physics.is_bottom_collision())
            {
                const auto& position = registry.get<PositionComponent>(owner);
                prefabs::SmokePuffParticle::create(position.x_center, position.y_center);
                registry.destroy(owner);
            }
        }
    private:
    };
}

entt::entity prefabs::BonesParticle::create()
{
    return create(0, 0);
}

entt::entity prefabs::BonesParticle::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 0.5f;
    const float height = 0.5f;

    PhysicsComponent physics(width, height);
    physics.set_gravity_modifier(0.6f);

    QuadComponent quad(TextureType::NPC, width, height);
    quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::BONES_PARTICLE_0_START);

    AnimationComponent animation;
    animation.start(static_cast<std::size_t>(NPCSpritesheetFrames::BONES_PARTICLE_0_START),
                    static_cast<std::size_t>(NPCSpritesheetFrames::BONES_PARTICLE_7_LAST),
                    100, true);

    MeshComponent mesh;
    mesh.rendering_layer = RenderingLayer::LAYER_4_DUDE;
    mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<AnimationComponent>(entity, animation);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<BonesParticleScript>());

    return entity;
}
