#include "components/generic/SaleableComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"
#include "EntityRegistry.hpp"
#include "prefabs/particles/RotatingDollarParticle.hpp"

namespace
{
    class DummyScript : public ScriptBase
    {
    public:
        void update(entt::entity owner, uint32_t delta_time_ms) override {}
        static std::shared_ptr<ScriptBase> make() { return std::make_shared<DummyScript>(); }
    };
}

SaleableComponent::SaleableComponent(int price_dollars, entt::entity parent_item, ZoneComponent shop_zone, PositionComponent shop_position)
        : _price_dollars(price_dollars)
        , _parent_item(parent_item)
        , _shop_zone(shop_zone)
        , _shop_position(shop_position)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& item_component = registry.get<ItemComponent>(parent_item);

    _original_item.slot = item_component.get_slot();
    _original_item.application = item_component.get_application();

    item_component.set_slot(ItemSlot::ACTIVE);
    item_component.set_type(ItemApplication::THROWABLE);

    if (registry.has<ScriptingComponent>(parent_item))
    {
        auto& script_component = registry.get<ScriptingComponent>(parent_item);
        _original_item.script = DummyScript::make();
        std::swap(script_component.script, _original_item.script);
    }

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
    _price_dollars = other._price_dollars;
    _original_item.script = other._original_item.script;
    _original_item.slot = other._original_item.slot;
    _original_item.application = other._original_item.application;
    _shop_zone = other._shop_zone;
    _shop_position = other._shop_position;

    return *this;
}

SaleableComponent::~SaleableComponent()
{
    if (_dollar_sign_animation != entt::null)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        if (registry.valid(_dollar_sign_animation))
        {
            registry.destroy(_dollar_sign_animation);
        }
    }
}

void SaleableComponent::update_position(PositionComponent &parent_item_position) const
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& dollar_animation_position = registry.get<PositionComponent>(_dollar_sign_animation);
    dollar_animation_position.x_center = parent_item_position.x_center;
    dollar_animation_position.y_center = parent_item_position.y_center - prefabs::RotatingDollarParticle::get_height();
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

    _dollar_sign_animation = prefabs::RotatingDollarParticle::create();
    update_position(parent_item_position);
}
