#pragma once

#include "prefabs/items/Compass.hpp"
#include "components/generic/ItemCarrierComponent.hpp"

class Viewport;

class HudCompassArrowObserver : public Observer<prefabs::CompassArrow>
{
public:
    explicit HudCompassArrowObserver(std::shared_ptr<Viewport>);
    ~HudCompassArrowObserver();
    void on_notify(const prefabs::CompassArrow *) override;
private:
    std::shared_ptr<Viewport> _viewport;
    entt::entity _arrow = entt::null;
};

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
    std::shared_ptr<HudCompassArrowObserver> _compass_arrow_observer;
    std::shared_ptr<Viewport> _viewport;
};
