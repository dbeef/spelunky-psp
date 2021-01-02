#include "EntityRegistry.hpp"
#include <prefabs/ui/HudOverlay.hpp>
#include "components/specialized/HudOverlayComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "TextureBank.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/MainMenuSpritesheetFrames.hpp"

entt::entity prefabs::HudOverlay::create(std::shared_ptr<Viewport> viewport)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    HudOverlayComponent hud(viewport);
    registry.emplace<HudOverlayComponent>(entity, hud);

    return entity;
}
