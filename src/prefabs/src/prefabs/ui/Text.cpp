#include "EntityRegistry.hpp"
#include "prefabs/ui/Text.hpp"

#include "components/generic/MeshComponent.hpp"
#include "components/generic/TextComponent.hpp"
#include "components/generic/PositionComponent.hpp"

#include "TextureBank.hpp"

entt::entity prefabs::Text::create()
{
    return create(0, 0, "");
}

entt::entity prefabs::Text::create(float x, float y, const std::string &contents)
{
    auto& registry = EntityRegistry::instance().get_registry();

    auto entity = registry.create();

    TextComponent text_component;
    PositionComponent position_component;
    MeshComponent mesh_component;

    position_component.x_center = x;
    position_component.y_center = y;
    text_component.set_text(contents);

    registry.emplace<TextComponent>(entity, text_component);
    registry.emplace<PositionComponent>(entity, position_component);
    registry.emplace<MeshComponent>(entity, mesh_component);

    return entity;
}
