#include "prefabs/other/Bones.hpp"

#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"

#include "spritesheet-frames/NPCSpritesheetFrames.hpp"
#include "EntityRegistry.hpp"
#include "TextureType.hpp"

entt::entity prefabs::Bones::create()
{
    return create(0, 0);
}

entt::entity prefabs::Bones::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 1.0f;

    PositionComponent position(pos_x_center, pos_y_center);
    QuadComponent quad(TextureType::NPC, width, height);
    MeshComponent mesh;

    quad.frame_changed(NPCSpritesheetFrames::BONES);
    mesh.rendering_layer = RenderingLayer::LAYER_4_DUDE;
    mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

    registry.emplace<PositionComponent>(entity, position);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);
    registry.emplace<PhysicsComponent>(entity, width, height);
    registry.emplace<HorizontalOrientationComponent>(entity);

    return entity;
}
