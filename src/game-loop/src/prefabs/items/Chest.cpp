#include "EntityRegistry.hpp"
#include "prefabs/items/Chest.hpp"

#include "components/generic/SortRenderingLayersComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"

#include "TextureBank.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

entt::entity prefabs::Chest::create()
{
    return create(0, 0);
}

entt::entity prefabs::Chest::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 1.0f;

    PositionComponent position(pos_x_center, pos_y_center);
    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    MeshComponent mesh;
    PhysicsComponent physics(width, height);

    quad.frame_changed(CollectiblesSpritesheetFrames::CHEST);
    mesh.rendering_layer = RenderingLayer::LAYER_4_PROPS;
    mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

    registry.emplace<PositionComponent>(entity, position);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);
    registry.emplace<PhysicsComponent>(entity, physics);

    {
        const auto entity = registry.create();
        registry.emplace<SortRenderingLayersComponent>(entity);
    }

    return entity;
}


