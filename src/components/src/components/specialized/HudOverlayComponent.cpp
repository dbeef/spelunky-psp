#include <utility>
#include <sstream>
#include <string>

#include "EntityRegistry.hpp"
#include "prefabs/ui/Text.hpp"
#include "prefabs/ui/HudIcon.hpp"

#include "components/generic/PositionComponent.hpp"
#include "components/specialized/HudOverlayComponent.hpp"
#include "spritesheet-frames/HUDSpritesheetFrames.hpp"
#include "other/Inventory.hpp"

namespace
{
    // std::to_string is missing in PSP's libc++.
    std::string to_string(uint32_t value)
    {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }
}

void HudOverlayComponent::update(uint32_t delta_time_ms)
{
    _dollars_buffer_count_timer += delta_time_ms;

    // TODO: User defined literal for milliseconds.
    if (_dollars_buffer_count_timer > 90)
    {
        _dollars_buffer_count_timer = 0;

        if (_dollars_buffer_count > 0)
        {
            const int diff = std::min<int>(100, _dollars_buffer_count);

            _dollars_count += diff;
            _dollars_buffer_count -= diff;

             update_dollars();
        }
    }

    _prompt_visibility_cooldown_ms -= delta_time_ms;
    if (_prompt_visibility_cooldown_ms < 0)
    {
        _prompt_visibility_cooldown_ms = 0;
        if (_texts.prompt != entt::null)
        {
            auto& registry = EntityRegistry::instance().get_registry();
            registry.destroy(_texts.prompt);
            _texts.prompt = entt::null;
        }
    }
}


HudOverlayComponent::HudOverlayComponent(HudOverlayComponent && other) noexcept
{
    *this = std::move(other);
}

HudOverlayComponent& HudOverlayComponent::operator=(HudOverlayComponent &&other) noexcept
{
    dispose_children();

    _item_observer = other._item_observer;
    _children = other._children;
    _texts = other._texts;

    other._children = {};
    other._texts = {};

    return *this;
}

HudOverlayComponent::HudOverlayComponent(const HudOverlayComponent& other)
    : _viewport(other._viewport)
    , _dollars_buffer_count(other._dollars_buffer_count)
    , _dollars_buffer_count_timer(other._dollars_buffer_count_timer)
    , _dollars_count(other._dollars_count)
    , _dollars_count_previously(other._dollars_count_previously)
{
    create_children();
    Inventory::instance().add_observer(this);
    _item_observer = std::make_shared<HudOverlayItemObserver>(_viewport);
}

HudOverlayComponent::HudOverlayComponent(std::shared_ptr<Viewport> viewport) : _viewport(std::move(viewport))
{
    create_children();
    Inventory::instance().add_observer(this);
    _item_observer = std::make_shared<HudOverlayItemObserver>(_viewport);
}

void HudOverlayComponent::on_notify(const ShoppingTransactionEvent * event)
{
    if (_texts.prompt == entt::null)
    {
        _texts.prompt = prefabs::Text::create(0, 0, "");
        _prompt_visibility_cooldown_ms = 2000;
    }

    auto& registry = EntityRegistry::instance().get_registry();

    auto& prompt_text = registry.get<TextComponent>(_texts.prompt);
    prompt_text.set_text(event->message);
    prompt_text.set_scale(0.75f);

    auto& position = registry.get<PositionComponent>(_texts.prompt);
    position.x_center = (_viewport->get_width_world_units() / 2.0f) -
                        (prompt_text.get_width() / 2.0f) +
                        (prompt_text.get_font_width() / 2.0f);
    position.y_center = _viewport->get_height_world_units() * 0.9f;
}

void HudOverlayComponent::on_notify(const InventoryEvent * event)
{
    auto& registry = EntityRegistry::instance().get_registry();

    switch(*event)
    {
        case InventoryEvent::HEARTS_COUNT_CHANGED:
        {
            auto& text = registry.get<TextComponent>(_texts.hearts);
            text.set_text(to_string(Inventory::instance().get_hearts()));
            break;
        }
        case InventoryEvent::ROPES_COUNT_CHANGED:
        {
            auto& text = registry.get<TextComponent>(_texts.ropes);
            text.set_text(to_string(Inventory::instance().get_ropes()));
            break;
        }
        case InventoryEvent::BOMBS_COUNT_CHANGED:
        {
            auto& text = registry.get<TextComponent>(_texts.bombs);
            text.set_text(to_string(Inventory::instance().get_bombs()));
            break;
        }
        case InventoryEvent::DOLLARS_COUNT_CHANGED:
        {
            int diff = Inventory::instance().get_dollars() - _dollars_count_previously;

            if (diff > 0)
            {
                _dollars_buffer_count += Inventory::instance().get_dollars() - _dollars_count_previously;
            }
            else
            {
                // Negative dollars balance i.e due to shopping transaction:
                _dollars_count = Inventory::instance().get_dollars();
            }

            _dollars_count_previously = Inventory::instance().get_dollars();
            update_dollars();
            break;
        }
        default: break;
    }
}

HudOverlayComponent::~HudOverlayComponent()
{
    Inventory::instance().remove_observer(this);
    dispose_children();
}

void HudOverlayComponent::update_dollars()
{
    auto& registry = EntityRegistry::instance().get_registry();

    auto &text_dollars_count = registry.get<TextComponent>(_texts.dollars);
    auto &position_dollars_count = registry.get<PositionComponent>(_texts.dollars);
    text_dollars_count.set_text(to_string(_dollars_count));

    if (_texts.dollars_buffer == entt::null)
    {
        {
            _texts.dollars_buffer = prefabs::Text::create();
            auto &text = registry.get<TextComponent>(_texts.dollars_buffer);

            text.set_scale(0.75f);
            text.set_yellow(true);
            text.set_text(to_string(Inventory::instance().get_dollars()));
        }
    }

    auto &text_dollars_buffer = registry.get<TextComponent>(_texts.dollars_buffer);
    auto &position_dollars_buffer = registry.get<PositionComponent>(_texts.dollars_buffer);

    text_dollars_buffer.set_text("+" + to_string(_dollars_buffer_count));
    position_dollars_buffer.x_center = position_dollars_count.x_center + text_dollars_count.get_width();
    position_dollars_buffer.y_center = position_dollars_count.y_center;


    if (_dollars_buffer_count == 0)
    {
        registry.destroy(_texts.dollars_buffer);
        _texts.dollars_buffer = entt::null;
    }
}

void HudOverlayComponent::create_children()
{
    auto& registry = EntityRegistry::instance().get_registry();

    assert(_viewport);

    const auto BASE_POS_X = static_cast<float>(_viewport->get_width_world_units() * 0.05f);
    const auto BASE_POS_Y = static_cast<float>(_viewport->get_height_world_units() * 0.05f);

    // Create icons:

    {
        const float x = BASE_POS_X + (ICONS_OFFSET_WORLD_UNITS * 1.5f * 0);
        const float y = BASE_POS_Y;
        _children.push_back(prefabs::HudIcon::create(x, y, HUDSpritesheetFrames::HEART));
    }

    {
        const float x = BASE_POS_X + (ICONS_OFFSET_WORLD_UNITS * 1.5f * 1);
        const float y = BASE_POS_Y;
        _children.push_back(prefabs::HudIcon::create(x, y, HUDSpritesheetFrames::BOMB_ICON));
    }

    {
        const float x = BASE_POS_X + (ICONS_OFFSET_WORLD_UNITS * 1.5f * 2);
        const float y = BASE_POS_Y;
        _children.push_back(prefabs::HudIcon::create(x, y, HUDSpritesheetFrames::ROPE_ICON));
    }

    {
        const float x = BASE_POS_X + (ICONS_OFFSET_WORLD_UNITS * 1.5f * 3);
        const float y = BASE_POS_Y;
        _children.push_back(prefabs::HudIcon::create(x, y, HUDSpritesheetFrames::DOLLAR_SIGN));
    }

    {
        const float x = BASE_POS_X + (ICONS_OFFSET_WORLD_UNITS * 0);
        const float y = BASE_POS_Y + (ICONS_OFFSET_WORLD_UNITS * 0.5f);
        _children.push_back(prefabs::HudIcon::create(x, y, HUDSpritesheetFrames::HOLD_ITEM_ICON));
    }

    // Create texts:

    {
        const float x = BASE_POS_X + (ICONS_OFFSET_WORLD_UNITS * 1.5f * 0) + prefabs::HudIcon::getIconSizeWorldUnits();
        const float y = BASE_POS_Y - (prefabs::HudIcon::getIconSizeWorldUnits() * 0.425);
        _texts.hearts = prefabs::Text::create(x, y, to_string(Inventory::instance().get_hearts()));
    }

    {
        const float x = BASE_POS_X + (ICONS_OFFSET_WORLD_UNITS * 1.5f * 1) + prefabs::HudIcon::getIconSizeWorldUnits();
        const float y = BASE_POS_Y - (prefabs::HudIcon::getIconSizeWorldUnits() * 0.425);
        _texts.bombs = prefabs::Text::create(x, y, to_string(Inventory::instance().get_bombs()));
    }

    {
        const float x = BASE_POS_X + (ICONS_OFFSET_WORLD_UNITS * 1.5f * 2) + prefabs::HudIcon::getIconSizeWorldUnits();
        const float y = BASE_POS_Y - (prefabs::HudIcon::getIconSizeWorldUnits() * 0.425);
        _texts.ropes = prefabs::Text::create(x, y, to_string(Inventory::instance().get_ropes()));
    }

    {
        const float x = BASE_POS_X + (ICONS_OFFSET_WORLD_UNITS * 1.5f * 3) + prefabs::HudIcon::getIconSizeWorldUnits();
        const float y = BASE_POS_Y - (prefabs::HudIcon::getIconSizeWorldUnits() * 0.4);
        _texts.dollars = prefabs::Text::create(x, y, to_string(Inventory::instance().get_dollars()));
    }

    // Update state:

    _dollars_count_previously = Inventory::instance().get_dollars();
    _dollars_count = Inventory::instance().get_dollars();

    update_dollars();
}

void HudOverlayComponent::dispose_children()
{
    auto& registry = EntityRegistry::instance().get_registry();

    // Check if valid, since registry may destroy these entities before destroying the HudOverlayComponent:
    if (registry.valid(_texts.prompt)) registry.destroy(_texts.prompt);
    if (registry.valid(_texts.dollars)) registry.destroy(_texts.dollars);
    if (registry.valid(_texts.hearts)) registry.destroy(_texts.hearts);
    if (registry.valid(_texts.ropes)) registry.destroy(_texts.ropes);
    if (registry.valid(_texts.bombs)) registry.destroy(_texts.bombs);
    if (registry.valid(_texts.dollars_buffer)) registry.destroy(_texts.dollars_buffer);

    for (auto& entity : _children)
    {
        if (registry.valid(entity)) registry.destroy(entity);
    }

    _texts = {};
    _children.clear();
}
