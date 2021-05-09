#pragma once

#include "components/generic/PositionComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/ZoneComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "entt/entt.hpp"

class SaleableComponent
{
public:
    SaleableComponent() = default;
    explicit SaleableComponent(int price_dollars, entt::entity parent_item, ZoneComponent shop_zone, PositionComponent shop_position);
    SaleableComponent(const SaleableComponent& other) noexcept;
    SaleableComponent(SaleableComponent&& other) noexcept;
    SaleableComponent& operator=(SaleableComponent&& other) noexcept;
    ~SaleableComponent();

    void update_position(PositionComponent& parent_item_position) const;
    void add_animation();

    ZoneComponent& get_shop_zone() { return _shop_zone; }
    PositionComponent& get_shop_position() { return _shop_position; }
    entt::entity get_parent_item() const { return _parent_item; }
    entt::entity get_dollar_sign_animation() const { return _dollar_sign_animation; }

    int get_price_dollars() const { return _price_dollars; }

    ItemApplication get_original_item_application() const { return _original_item.application; }
    ItemSlot get_original_item_slot() const { return _original_item.slot; }
    std::shared_ptr<ScriptBase> get_original_script() { return _original_item.script; }

private:
    entt::entity _parent_item = entt::null;
    entt::entity _dollar_sign_animation = entt::null;
    int _price_dollars = 0;

    struct
    {
        ItemApplication application{};
        ItemSlot slot{};
        std::shared_ptr<ScriptBase> script = nullptr;
    } _original_item;

    ZoneComponent _shop_zone;
    PositionComponent _shop_position;
};
