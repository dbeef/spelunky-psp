#include "prefabs/collectibles/TripleGoldBar.hpp"

#include "components/generic/CollectibleComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

entt::entity prefabs::TripleGoldBar::create()
{
    return create(0, 0);
}

entt::entity prefabs::TripleGoldBar::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 1.0f;

    PositionComponent position(pos_x_center, pos_y_center);
    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    MeshComponent mesh;
    PhysicsComponent physics(width, height);
    CollectibleComponent collectible(1000, LootCollectedEvent::TRIPLE_GOLD_BAR);

    quad.frame_changed(CollectiblesSpritesheetFrames::TRIPLE_GOLD_BAR);
    mesh.rendering_layer = RenderingLayer::LAYER_5_PROPS;
    mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

    registry.emplace<PositionComponent>(entity, position);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<CollectibleComponent>(entity, collectible);

    return entity;
}
