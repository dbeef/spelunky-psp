#pragma once

#include "components/generic/MeshComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/PositionComponent.hpp"

#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"
#include "EntityRegistry.hpp"
#include "entt/entt.hpp"

// TODO: HUD should subscribe to saleable components? Saleable component / ShoppingSystem would check if saleable
//       item is held, and if so, would notify all observers?
//
// Maybe it should be the other way around, HUD observing the Wallet passive item, and Wallet checking for saleable
// items in active slot of ItemCarrierComponent?

struct SaleableItemPickedUpEvent
{
    const char* msg;
};

class SaleableComponent
{
public:
    SaleableComponent() = default;

    explicit SaleableComponent(int price_dollars, entt::entity parent_shopkeeper, entt::entity parent_item)
        : _price_dollars(price_dollars)
        , _parent_shopkeeper(parent_shopkeeper)
        , _parent_item(parent_item)
    {
        add_animation();
    }

    SaleableComponent(const SaleableComponent& other) noexcept
    {
        add_animation();
    }

    SaleableComponent(SaleableComponent&& other) noexcept
    {
        *this = std::move(other);
    }

    SaleableComponent& operator=(SaleableComponent&& other) noexcept
    {
        _dollar_sign_animation = other._dollar_sign_animation;
        other._dollar_sign_animation = entt::null;

        _parent_item = other._parent_item;
        _parent_shopkeeper = other._parent_shopkeeper;

        return *this;
    }

    ~SaleableComponent()
    {
        if (_dollar_sign_animation != entt::null)
        {
            auto& registry = EntityRegistry::instance().get_registry();
            registry.destroy(_dollar_sign_animation);
        }
    }

    void update_position(PositionComponent& parent_item_position) const
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& dollar_animation_position = registry.get<PositionComponent>(_dollar_sign_animation);
        dollar_animation_position.x_center = parent_item_position.x_center;
        dollar_animation_position.y_center = parent_item_position.y_center - height;
    }

    void add_animation()
    {
        // FIXME: Move semantics
        //if (_dollar_sign_animation != entt::null)
        //{
        //    return;
        //}

        assert(_parent_item != entt::null);

        auto& registry = EntityRegistry::instance().get_registry();
        auto& parent_item_position = registry.get<PositionComponent>(_parent_item);

        _dollar_sign_animation = registry.create();

        AnimationComponent animation;
        animation.start(static_cast<int>(CollectiblesSpritesheetFrames::SHOP_DOLLAR_ANIMATION_0_FIRST),
                        static_cast<int>(CollectiblesSpritesheetFrames::SHOP_DOLLAR_ANIMATION_19_LAST),
                        50, true);

        registry.emplace<MeshComponent>(_dollar_sign_animation, RenderingLayer::LAYER_3_ITEMS, CameraType::MODEL_VIEW_SPACE);
        registry.emplace<PositionComponent>(_dollar_sign_animation, parent_item_position.x_center, parent_item_position.y_center - height);
        registry.emplace<QuadComponent>(_dollar_sign_animation, TextureType::COLLECTIBLES, width, height);
        registry.emplace<AnimationComponent>(_dollar_sign_animation, animation);
    }

    entt::entity get_parent_item() const { return _parent_item; }
    entt::entity get_dollar_sign_animation() const { return _dollar_sign_animation; }

private:
    entt::entity _parent_shopkeeper = entt::null;
    entt::entity _parent_item = entt::null;
    entt::entity _dollar_sign_animation = entt::null;
    int _price_dollars = 0;

    const float width = 8.0 / 16.0f;
    const float height = 10.0 / 16.0f;
};
