#include "prefabs/collectibles/SmallGem.hpp"

#include "components/generic/CollectibleComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"

#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"
#include "EntityRegistry.hpp"

entt::entity prefabs::SmallGem::create()
{
    return create(0, 0);
}

entt::entity prefabs::SmallGem::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 0.25f;
    const float height = 0.25f;

    PositionComponent position(pos_x_center, pos_y_center);
    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    MeshComponent mesh;
    PhysicsComponent physics(width, height);
    CollectibleComponent collectible(1000, LootCollectedEvent::SMALL_GEM);

    std::uint16_t frame_index = static_cast<std::uint16_t>(CollectiblesSpritesheetFrames::DIAMOND_SMALL) + std::rand() % 3;

    quad.frame_changed(static_cast<CollectiblesSpritesheetFrames>(frame_index));
    mesh.rendering_layer = RenderingLayer::LAYER_5_PROPS;
    mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

    registry.emplace<PositionComponent>(entity, position);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<CollectibleComponent>(entity, collectible);

    return entity;
}

