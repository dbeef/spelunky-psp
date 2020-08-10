#pragma once

#include <memory>
#include <string>

#include "GameEntity.hpp"
#include "components/TextComponent.hpp"
#include "viewport/Viewport.hpp"

class LevelSummaryOverlay : public GameEntity
{
public:

    explicit LevelSummaryOverlay(std::shared_ptr<Viewport>);
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
};
