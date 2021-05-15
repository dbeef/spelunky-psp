#include "prefabs/props/GithubLink.hpp"

#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/MainMenuSpritesheetFrames.hpp"

entt::entity prefabs::GithubLink::create()
{
    return create(0, 0);
}

entt::entity prefabs::GithubLink::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    PositionComponent position(pos_x_center, pos_y_center);
    QuadComponent quad(TextureType::MAIN_MENU, 8.0f, 1.0);
    MeshComponent mesh;

    quad.frame_changed(MainMenuSpritesheetFrames::GITHUB_LINK);
    mesh.rendering_layer = RenderingLayer::LAYER_5_PROPS;
    mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

    registry.emplace<PositionComponent>(entity, position);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);

    return entity;
}
