#include "prefabs/items/RopeChainElement.hpp"

#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"

#include "EntityRegistry.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"
#include "TextureType.hpp"

entt::entity prefabs::RopeChainElement::create()
{
    return create(0, 0);
}

entt::entity prefabs::RopeChainElement::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 0.5f;

    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    quad.frame_changed(CollectiblesSpritesheetFrames::ROPE_CHAIN_ELEMENT);

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_5_PROPS, CameraType::MODEL_VIEW_SPACE);

    return entity;
}
