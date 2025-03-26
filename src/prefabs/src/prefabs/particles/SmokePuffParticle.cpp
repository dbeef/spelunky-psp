#include "prefabs/particles/SmokePuffParticle.hpp"

#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/TimeLimitComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

entt::entity prefabs::SmokePuffParticle::create()
{
    return create(0, 0);
}

entt::entity prefabs::SmokePuffParticle::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    PositionComponent position(pos_x_center, pos_y_center);
    QuadComponent quad(TextureType::COLLECTIBLES, 0.5f, 0.5f);
    MeshComponent mesh;
    AnimationComponent animation;

    quad.frame_changed<CollectiblesSpritesheetFrames>(CollectiblesSpritesheetFrames::SMOKE_PUFF_0_FIRST);

    animation.start(static_cast<std::size_t>(CollectiblesSpritesheetFrames::SMOKE_PUFF_0_FIRST),
                    static_cast<std::size_t>(CollectiblesSpritesheetFrames::SMOKE_PUFF_7_LAST),
                    75, false);

    mesh.rendering_layer = RenderingLayer::LAYER_4_DUDE;
    mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

    registry.emplace<PositionComponent>(entity, position);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);
    registry.emplace<AnimationComponent>(entity, animation);
    registry.emplace<TimeLimitComponent>(entity, 650);

    return entity;
}
