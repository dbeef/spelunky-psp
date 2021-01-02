#include "EntityRegistry.hpp"
#include "components/specialized/LevelSummaryOverlayComponent.hpp"
#include "prefabs/ui/LevelSummaryOverlay.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "TextureBank.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/MainMenuSpritesheetFrames.hpp"

entt::entity prefabs::LevelSummaryOverlay::create(std::shared_ptr<Viewport> viewport,
                                                  const std::shared_ptr<LevelSummaryTracker>& level_summary_tracker)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    LevelSummaryOverlayComponent level_summary(viewport, level_summary_tracker);
    registry.emplace<LevelSummaryOverlayComponent>(entity, level_summary);

    return entity;    
}
