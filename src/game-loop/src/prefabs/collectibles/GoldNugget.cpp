#include "prefabs/collectibles/GoldNugget.hpp"

#include "components/generic/CollectibleComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

entt::entity prefabs::GoldNugget::create()
{
    return create(0, 0);
}

entt::entity prefabs::GoldNugget::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 0.5f;
    const float height = 0.5f;

    PositionComponent position(pos_x_center, pos_y_center);
    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    MeshComponent mesh;
    PhysicsComponent physics(width, height);
    CollectibleComponent collectible(500, LootCollectedEvent::GOLD_NUGGET);

    quad.frame_changed(CollectiblesSpritesheetFrames::GOLD_NUGGET);
    mesh.rendering_layer = RenderingLayer::LAYER_5_PROPS;
    mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

    registry.emplace<PositionComponent>(entity, position);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<CollectibleComponent>(entity, collectible);

    return entity;
}
