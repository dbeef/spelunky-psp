#pragma once

#include <memory>

#include "components/TextComponent.hpp"
#include "GameEntity.hpp"
#include "components/QuadComponent.hpp"
#include "Point2D.hpp"
#include "viewport/Viewport.hpp"
#include "patterns/Observer.hpp"
#include "main-dude/MainDudeEvent.hpp"
#include "other/Inventory.hpp"

class MainDude;

class HUD : public GameEntity, public Observer<InventoryEvent >
{
public:

    HUD(std::shared_ptr<Viewport>);
    ~HUD() override;

    void update(uint32_t delta_time_ms) override;
    void on_notify(const InventoryEvent *) override;

    void hide();

private:

    std::shared_ptr<Viewport> _viewport;

    struct
    {
        std::shared_ptr<QuadComponent> heart = nullptr;
        std::shared_ptr<QuadComponent> dollar = nullptr;
        std::shared_ptr<QuadComponent> ropes = nullptr;
        std::shared_ptr<QuadComponent> bombs = nullptr;
        std::shared_ptr<QuadComponent> hold_item = nullptr;
    } _quads;

    struct
    {
        TextComponent hearts;
        TextComponent dollars;
        TextComponent ropes;
        TextComponent bombs;
    } _texts;
};
