#include "EntityRegistry.hpp"
#include <components/specialized/PauseOverlayComponent.hpp>
#include "prefabs/ui/PauseOverlay.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "TextureBank.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/MainMenuSpritesheetFrames.hpp"

entt::entity prefabs::PauseOverlay::create(const std::shared_ptr<Viewport>& viewport, PauseOverlayComponent::Type type)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    PauseOverlayComponent pause(viewport, type);
    registry.emplace<PauseOverlayComponent>(entity, pause);

    return entity;
}
