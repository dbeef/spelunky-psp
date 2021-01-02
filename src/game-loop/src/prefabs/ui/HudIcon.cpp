#include <components/generic/PositionComponent.hpp>
#include <TextureType.hpp>
#include <components/generic/MeshComponent.hpp>
#include <components/generic/QuadComponent.hpp>
#include <spritesheet-frames/HUDSpritesheetFrames.hpp>
#include "prefabs/ui/HudIcon.hpp"
#include "EntityRegistry.hpp"

entt::entity prefabs::HudIcon::create(float x, float y, HUDSpritesheetFrames frame)
{
    auto& registry = EntityRegistry::instance().get_registry();

    auto entity = registry.create();

    PositionComponent position(x, y);
    QuadComponent quad(TextureType::HUD, getIconSizeWorldUnits(), getIconSizeWorldUnits());
    MeshComponent mesh;

    mesh.rendering_layer = RenderingLayer::LAYER_2_UI_QUADS;
    mesh.camera_type = CameraType::SCREEN_SPACE;
    quad.frame_changed<HUDSpritesheetFrames>(frame);

    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<PositionComponent>(entity, position);
    registry.emplace<MeshComponent>(entity, mesh);

    return entity;
}

float prefabs::HudIcon::getIconSizeWorldUnits()
{
    return 0.5f;
}
