#pragma once

#include <memory>
#include <string>
#include <vector>

#include <entt/entt.hpp>


#include "components/generic/TextComponent.hpp"

class LevelSummaryOverlayComponent;
class LevelSummaryTracker;

class LevelSummaryOverlayComponent
{
public:

    LevelSummaryOverlayComponent(const std::shared_ptr<Viewport>&, const std::shared_ptr<LevelSummaryTracker>&);
    void update(uint32_t delta_time_ms);

private:

    uint32_t _kills_appearing_timer = 0;
    uint32_t _kills_spawned = 0;

    uint32_t _loot_appearing_timer = 0;
    uint32_t _loot_spawned = 0;

    std::shared_ptr<Viewport> _viewport;
    std::shared_ptr<LevelSummaryTracker> _level_summary_tracker;
};
