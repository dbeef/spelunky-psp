#pragma once

#include <memory>
#include <string>

#include "GameEntity.hpp"
#include "components/TextComponent.hpp"
#include "viewport/Viewport.hpp"

class ScoresOverlay : public GameEntity
{
public:

    explicit ScoresOverlay(std::shared_ptr<Viewport>);
    void update(uint32_t delta_time_ms) override;

private:

    std::shared_ptr<Viewport> _viewport;

    struct
    {
        TextComponent top_defilers;
        TextComponent money;
        TextComponent kills;
        TextComponent saves;
        TextComponent statistics;
        TextComponent plays;
        TextComponent deaths;
        TextComponent wins;
    } _texts;
};
