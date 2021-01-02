#pragma once

#include <memory>
#include <vector>

#include <entt/entt.hpp>
#include <components/specialized/HudOverlayComponent.hpp>

class GameEntity;
class LevelSummaryTracker;

namespace populator
{
    void generate_loot(std::shared_ptr<LevelSummaryTracker>&);
}
