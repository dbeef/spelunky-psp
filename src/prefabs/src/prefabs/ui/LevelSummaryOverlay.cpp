#include "prefabs/ui/LevelSummaryOverlay.hpp"
#include "components/specialized/LevelSummaryOverlayComponent.hpp"
#include "EntityRegistry.hpp"

entt::entity prefabs::LevelSummaryOverlay::create(std::shared_ptr<Viewport> viewport,
                                                  const std::shared_ptr<LevelSummaryTracker>& level_summary_tracker)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    LevelSummaryOverlayComponent level_summary(viewport, level_summary_tracker);
    registry.emplace<LevelSummaryOverlayComponent>(entity, level_summary);

    return entity;    
}
