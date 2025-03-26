#pragma once

#include <components/specialized/LevelSummaryTracker.hpp>
#include <viewport/Viewport.hpp>
#include "entt/entt.hpp"
#include "components/specialized/LevelSummaryOverlayComponent.hpp"

namespace prefabs
{
    struct LevelSummaryOverlay
    {
        static entt::entity create(std::shared_ptr<Viewport>, const std::shared_ptr<LevelSummaryTracker>&);
    };
}
