#include "prefabs/particles/FlareSparkParticle.hpp"

#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/TimeLimitComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

entt::entity prefabs::FlareSparkParticle::create()
{
    return create(0, 0);
}

entt::entity prefabs::FlareSparkParticle::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 0.25f;
    const float height = 0.25f;

    PositionComponent position(pos_x_center, pos_y_center);
    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    MeshComponent mesh;
    AnimationComponent animation;

    quad.frame_changed<CollectiblesSpritesheetFrames>(CollectiblesSpritesheetFrames::FLARE_SPARK_0_FIRST);

    animation.start(static_cast<std::size_t>(CollectiblesSpritesheetFrames::FLARE_SPARK_0_FIRST),
                    static_cast<std::size_t>(CollectiblesSpritesheetFrames::FLARE_SPARK_4_LAST),
                    110, true);

    mesh.rendering_layer = RenderingLayer::LAYER_4_DUDE;
    mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

    registry.emplace<PositionComponent>(entity, position);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);
    registry.emplace<AnimationComponent>(entity, animation);
    registry.emplace<TimeLimitComponent>(entity, 550);

    return entity;
}
