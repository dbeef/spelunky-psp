#include "EntityRegistry.hpp"
#include "components/specialized/DeathOverlayComponent.hpp"
#include "prefabs/ui/DeathOverlay.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "TextureBank.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/MainMenuSpritesheetFrames.hpp"

entt::entity prefabs::DeathOverlay::create(const std::shared_ptr<Viewport>& viewport)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    DeathOverlayComponent death(viewport);
    registry.emplace<DeathOverlayComponent>(entity, death);

    return entity;
}
