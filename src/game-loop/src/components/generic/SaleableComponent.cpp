#include "components/generic/SaleableComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"
#include "EntityRegistry.hpp"

SaleableComponent::SaleableComponent(int price_dollars, entt::entity parent_shopkeeper, entt::entity parent_item)
        : _price_dollars(price_dollars)
        , _parent_shopkeeper(parent_shopkeeper)
        , _parent_item(parent_item)
{
    add_animation();
}

SaleableComponent::SaleableComponent(const SaleableComponent &other) noexcept
{
    add_animation();
}

SaleableComponent::SaleableComponent(SaleableComponent &&other) noexcept
{
    *this = std::move(other);
    add_animation();
}

SaleableComponent &SaleableComponent::operator=(SaleableComponent &&other) noexcept
{
    _parent_item = other._parent_item;
    _parent_shopkeeper = other._parent_shopkeeper;
    _price_dollars = other._price_dollars;

    return *this;
}

SaleableComponent::~SaleableComponent()
{
    if (_dollar_sign_animation != entt::null)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        registry.destroy(_dollar_sign_animation);
    }
}

void SaleableComponent::update_position(PositionComponent &parent_item_position) const
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& dollar_animation_position = registry.get<PositionComponent>(_dollar_sign_animation);
    dollar_animation_position.x_center = parent_item_position.x_center;
    dollar_animation_position.y_center = parent_item_position.y_center - height;
}

void SaleableComponent::add_animation()
{
    // FIXME: Move semantics
    if (_dollar_sign_animation != entt::null)
    {
        return;
    }

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
