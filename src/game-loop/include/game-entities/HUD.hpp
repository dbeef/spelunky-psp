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

    int _dollars_buffer_count = 0;
    uint32_t _dollars_buffer_count_timer = 0;
    uint32_t _dollars_count_previously = 0;
    uint32_t _dollars_count = 0;

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
        TextComponent dollars_buffer;
        TextComponent ropes;
        TextComponent bombs;
    } _texts;

    void update_dollars();
};
