#pragma once

#include <memory>
#include <entt/entt.hpp>

#include "components/generic/TextComponent.hpp"

#include "Point2D.hpp"
#include "viewport/Viewport.hpp"
#include "patterns/Observer.hpp"
#include "main-dude/MainDudeEvent.hpp"
#include "other/Inventory.hpp"
#include "components/generic/ItemCarrierComponent.hpp"

#include <memory>
#include <utility>

class MainDudeComponent;

class HudOverlayItemObserver : public Observer<ItemCarrierEvent>
{
public:
    explicit HudOverlayItemObserver(std::shared_ptr<Viewport>);
    ~HudOverlayItemObserver();
    void on_notify(const ItemCarrierEvent *) override;
    int get_displayed_items() const { return _displayed_items; }
private:
    bool displays_item(ItemType) const;
    Point2D get_item_icon_position(std::size_t index) const;
    int _displayed_items = 0;
    std::vector<std::pair<ItemType, entt::entity>> _children;
    std::shared_ptr<Viewport> _viewport;
};

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
