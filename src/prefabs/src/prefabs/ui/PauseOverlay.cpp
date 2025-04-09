#include "prefabs/ui/PauseOverlay.hpp"
#include "components/specialized/PauseOverlayComponent.hpp"
#include "EntityRegistry.hpp"

entt::entity prefabs::PauseOverlay::create(const std::shared_ptr<Viewport>& viewport, PauseOverlayComponent::Type type)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    PauseOverlayComponent pause(viewport, type);
    registry.emplace<PauseOverlayComponent>(entity, pause);

    return entity;
}
