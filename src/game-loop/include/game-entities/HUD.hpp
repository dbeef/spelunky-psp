#pragma once

#include <memory>

#include "components/TextComponent.hpp"
#include "GameEntity.hpp"
#include "components/QuadComponent.hpp"
#include "Point2D.hpp"
#include "viewport/Viewport.hpp"

class HUD : public GameEntity
{
public:

    explicit HUD(std::shared_ptr<Viewport>);

    void update(uint32_t delta_time_ms) override;

    void set_hearts_count(uint32_t hearts);
    void set_ropes_count(uint32_t ropes);
    void set_bombs_count(uint32_t bombs);
    void set_dollars_count(uint32_t dollars);

private:

    QuadComponent _heart_quad;
    QuadComponent _dollar_quad;
    QuadComponent _ropes_quad;
    QuadComponent _bombs_quad;
    QuadComponent _hold_item_quad;

    std::shared_ptr<Viewport> _viewport;

    struct
    {
        TextComponent hearts;
        TextComponent dollars;
        TextComponent ropes;
        TextComponent bombs;
    } _texts;
};
