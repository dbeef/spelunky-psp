#pragma once

#include <memory>
#include <entt/entt.hpp>

#include "Point2D.hpp"
#include "viewport/Viewport.hpp"
#include "patterns/Observer.hpp"
#include "main-dude/MainDudeEvent.hpp"
#include "other/Inventory.hpp"
#include "components/generic/TextComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
#include "components/specialized/HudOverlayObservers.hpp"

#include <memory>
#include <utility>

static const float ICONS_OFFSET_WORLD_UNITS = 1.5f;

class MainDudeComponent;

class HudOverlayComponent : public Observer<InventoryEvent>
{
public:

    explicit HudOverlayComponent(std::shared_ptr<Viewport>);
    HudOverlayComponent(const HudOverlayComponent&);
    HudOverlayComponent(HudOverlayComponent&&) noexcept;
    HudOverlayComponent& operator=(HudOverlayComponent&& other) noexcept;

    ~HudOverlayComponent() override;

    void update(uint32_t delta_time_ms);
    void on_notify(const InventoryEvent *) override;
    HudOverlayItemObserver* get_item_observer() { return _item_observer.get(); }

private:

    std::shared_ptr<Viewport> _viewport;
    std::shared_ptr<HudOverlayItemObserver> _item_observer;

    int _dollars_buffer_count = 0;
    uint32_t _dollars_buffer_count_timer = 0;
    uint32_t _dollars_count_previously = 0;
    uint32_t _dollars_count = 0;

    std::vector<entt::entity> _children;

    struct
    {
        entt::entity hearts = entt::null;
        entt::entity dollars = entt::null;
        entt::entity dollars_buffer = entt::null;
        entt::entity ropes = entt::null;
        entt::entity bombs = entt::null;
    } _texts;

    void dispose_children();
    void create_children();
    void update_dollars();
};
