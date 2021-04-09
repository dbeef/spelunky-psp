#include "prefabs/effects/ShotgunBlast.hpp"

#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/TimeLimitComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

entt::entity prefabs::ShotgunBlast::create()
{
    return create(0, 0);
}

entt::entity prefabs::ShotgunBlast::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    PositionComponent position(pos_x_center, pos_y_center);
    QuadComponent quad(TextureType::COLLECTIBLES, 1.0f, 1.0f);
    MeshComponent mesh;
    AnimationComponent animation;

    quad.frame_changed<CollectiblesSpritesheetFrames>(CollectiblesSpritesheetFrames::SHOTGUN_BLAST_0_FIRST);

    animation.start(static_cast<std::size_t>(CollectiblesSpritesheetFrames::SHOTGUN_BLAST_0_FIRST),
                    static_cast<std::size_t>(CollectiblesSpritesheetFrames::SHOTGUN_BLAST_9_LAST),
                    40, false);

    mesh.rendering_layer = RenderingLayer::LAYER_3_ITEMS;
    mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

    registry.emplace<PositionComponent>(entity, position);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);
    registry.emplace<AnimationComponent>(entity, animation);
    registry.emplace<TimeLimitComponent>(entity, 400);
    registry.emplace<HorizontalOrientationComponent>(entity);

    return entity;
}
