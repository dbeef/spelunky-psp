#include "prefabs/traps/Spikes.hpp"

#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/damage/TakeExplosionDamageComponent.hpp"
#include "components/damage/HitpointComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/NPCSpritesheetFrames.hpp"

namespace
{
    class SpikesScript final : public ScriptBase
    {
    public:
        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            // TODO
        }
    };
}

entt::entity prefabs::Spikes::create()
{
    return create(0, 0);
}

entt::entity prefabs::Spikes::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 1.0f;

    PositionComponent position(pos_x_center, pos_y_center);
    QuadComponent quad(TextureType::NPC, width, height);
    MeshComponent mesh;
    PhysicsComponent physics(width, height);

    physics.disable_gravity();

    quad.frame_changed(NPCSpritesheetFrames::SPIKES);
    mesh.rendering_layer = RenderingLayer::LAYER_2_ITEMS;
    mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

    registry.emplace<PositionComponent>(entity, position);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<SpikesScript>());
    registry.emplace<HitpointComponent>(entity, 1, true);
    registry.emplace<TakeExplosionDamageComponent>(entity);

    // TODO: Spikes should take explosion damage (but without arrow-trap's checking for tile, only hitpoint component)
    //       ^ DONE

    // TODO: Spikes should give melee damage to every entity that collides with it AND has positive vertical speed (some treshold?)
    //       New kind of component? [Give/Take]SpikeDamageComponent?
    //       Threshold speed could make cape item save from killing by spikes - like in the original game

    return entity;
}
