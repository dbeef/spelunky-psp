#include "prefabs/particles/RotatingDollarParticle.hpp"

#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/AnimationComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

entt::entity prefabs::RotatingDollarParticle::create()
{
    return create(0, 0);
}

entt::entity prefabs::RotatingDollarParticle::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    PositionComponent position(pos_x_center, pos_y_center);
    QuadComponent quad(TextureType::COLLECTIBLES, get_width(), get_height());
    MeshComponent mesh;
    AnimationComponent animation;

    quad.frame_changed<CollectiblesSpritesheetFrames>(CollectiblesSpritesheetFrames::SHOP_DOLLAR_ANIMATION_0_FIRST);

    animation.start(static_cast<int>(CollectiblesSpritesheetFrames::SHOP_DOLLAR_ANIMATION_0_FIRST),
                    static_cast<int>(CollectiblesSpritesheetFrames::SHOP_DOLLAR_ANIMATION_19_LAST),
                    50, true);

    mesh.rendering_layer = RenderingLayer::LAYER_3_ITEMS;
    mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

    registry.emplace<PositionComponent>(entity, position);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);
    registry.emplace<AnimationComponent>(entity, animation);

    return entity;
}

float prefabs::RotatingDollarParticle::get_width()
{
    return 8.0 / 16.0f;
}

float prefabs::RotatingDollarParticle::get_height()
{
    return 10.0 / 16.0f;
}
