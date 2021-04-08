#include "prefabs/effects/Explosion.hpp"

#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/TimeLimitComponent.hpp"
#include "components/generic/ZoneComponent.hpp"
#include "components/damage/GiveExplosionDamageComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

entt::entity prefabs::Explosion::create()
{
    return create(0, 0);
}

entt::entity prefabs::Explosion::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 4.0f;
    const float height = 4.0f;

    PositionComponent position(pos_x_center, pos_y_center);
    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    MeshComponent mesh;
    AnimationComponent animation;

    quad.frame_changed<CollectiblesSpritesheetFrames>(CollectiblesSpritesheetFrames::EXPLOSION_0_FIRST);

    animation.start(static_cast<std::size_t>(CollectiblesSpritesheetFrames::EXPLOSION_0_FIRST),
                    static_cast<std::size_t>(CollectiblesSpritesheetFrames::EXPLOSION_9_LAST),
                    40, false);

    mesh.rendering_layer = RenderingLayer::LAYER_2_ITEMS;
    mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

    registry.emplace<PositionComponent>(entity, position);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);
    registry.emplace<AnimationComponent>(entity, animation);
    registry.emplace<TimeLimitComponent>(entity, 400);
    registry.emplace<ZoneComponent>(entity, width, height);
    registry.emplace<GiveExplosionDamageComponent>(entity);

    // TODO: ZoneComponent (consisting of just dimensions)
    // TODO: Bomb explosion should create an entity consisting of:
    //       * ZoneComponent <- To have area of influence
    //       * PositionComponent <- ditto
    //       * TimeLimitComponent <- To dissappear over time
    //       * GiveNpcDamageComponent <- To kill main dude
    //       * GiveMeleeDamageComponent <- To kill NPC's
    //
    // ^ Or specifically, GiveExplosionDamageComponent? Should it require implementing new observers?
    // Or, GiveZoneDamageComponent/TakeZoneDamageComponent
    // Technically bombs give kill instantly (max damage), so they are unique in this respect.
    // Should zone be used inside physics component? Function for checking overlap?
    // Or, just use PhysicsComponent, since it defines w/h, just disable gravity?
    //
    // Should width/height be excluded from physics component? Theoretically no, as w/h is integral part of PhysicsComponent and
    // makes no sense without it

    return entity;
}
