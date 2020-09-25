#pragma once

#include <memory>
#include <string>
#include <vector>

#include "GameEntity.hpp"
#include "components/TextComponent.hpp"

class LevelSummaryOverlay;
class LevelSummaryTracker;

class LevelSummaryOverlay : public GameEntity
{
public:

    LevelSummaryOverlay(std::shared_ptr<Viewport>, const std::shared_ptr<LevelSummaryTracker>&);
    void update(uint32_t delta_time_ms) override;

private:

    std::shared_ptr<Viewport> _viewport;

    struct
    {
        TextComponent level_completed;
        TextComponent time;
        TextComponent loot;
        TextComponent kills;
        TextComponent money;
    } _texts;

    uint32_t _loot_appearing_timer = 0;
    std::vector<std::shared_ptr<GameEntity>> _loot_entities;
};
