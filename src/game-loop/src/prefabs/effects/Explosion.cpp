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

    mesh.rendering_layer = RenderingLayer::LAYER_3_ITEMS;
    mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

    registry.emplace<PositionComponent>(entity, position);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);
    registry.emplace<AnimationComponent>(entity, animation);
    registry.emplace<TimeLimitComponent>(entity, 400);
    registry.emplace<ZoneComponent>(entity, width, height);
    registry.emplace<GiveExplosionDamageComponent>(entity);

    return entity;
}
