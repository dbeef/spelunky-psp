#include <utility>
#include <sstream>
#include <string>
#include "EntityRegistry.hpp"
#include "prefabs/ui/Text.hpp"
#include "prefabs/ui/HudIcon.hpp"

#include <components/generic/PositionComponent.hpp>
#include <components/generic/MeshComponent.hpp>
#include <components/specialized/HudOverlayComponent.hpp>
#include "spritesheet-frames/HUDSpritesheetFrames.hpp"
#include "other/Inventory.hpp"

namespace
{
    const float ICONS_OFFSET_WORLD_UNITS = 1.5f;

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
}


HudOverlayComponent::HudOverlayComponent(HudOverlayComponent && other) noexcept
{
    *this = std::move(other);
}

HudOverlayComponent& HudOverlayComponent::operator=(HudOverlayComponent &&other) noexcept
{
    dispose_children();

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
}

HudOverlayComponent::HudOverlayComponent(std::shared_ptr<Viewport> viewport) : _viewport(std::move(viewport))
{
    create_children();
    Inventory::instance().add_observer(this);
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
        case InventoryEvent::DOLLARS_COUNT_CHANGED:
        {
            _dollars_buffer_count += Inventory::instance().get_dollars() - _dollars_count_previously;
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
        const float x = BASE_POS_X + (ICONS_OFFSET_WORLD_UNITS * 0);
        const float y = BASE_POS_Y;
        _children.push_back(prefabs::HudIcon::create(x, y, HUDSpritesheetFrames::HEART));
    }

    {
        const float x = BASE_POS_X + (ICONS_OFFSET_WORLD_UNITS * 1);
        const float y = BASE_POS_Y;
        _children.push_back(prefabs::HudIcon::create(x, y, HUDSpritesheetFrames::BOMB_ICON));
    }

    {
        const float x = BASE_POS_X + (ICONS_OFFSET_WORLD_UNITS * 2);
        const float y = BASE_POS_Y;
        _children.push_back(prefabs::HudIcon::create(x, y, HUDSpritesheetFrames::ROPE_ICON));
    }

    {
        const float x = BASE_POS_X + (ICONS_OFFSET_WORLD_UNITS * 3);
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
        const float x = BASE_POS_X + (ICONS_OFFSET_WORLD_UNITS * 0) + prefabs::HudIcon::getIconSizeWorldUnits();
        const float y = BASE_POS_Y - (prefabs::HudIcon::getIconSizeWorldUnits() * 0.425);
        _texts.hearts = prefabs::Text::create(x, y, to_string(Inventory::instance().get_hearts()));
    }

    {
        const float x = BASE_POS_X + (ICONS_OFFSET_WORLD_UNITS * 1) + prefabs::HudIcon::getIconSizeWorldUnits();
        const float y = BASE_POS_Y - (prefabs::HudIcon::getIconSizeWorldUnits() * 0.425);
        _texts.ropes = prefabs::Text::create(x, y, to_string(Inventory::instance().get_ropes()));
    }

    {
        const float x = BASE_POS_X + (ICONS_OFFSET_WORLD_UNITS * 2) + prefabs::HudIcon::getIconSizeWorldUnits();
        const float y = BASE_POS_Y - (prefabs::HudIcon::getIconSizeWorldUnits() * 0.425);
        _texts.bombs = prefabs::Text::create(x, y, to_string(Inventory::instance().get_bombs()));
    }

    {
        const float x = BASE_POS_X + (ICONS_OFFSET_WORLD_UNITS * 3) + prefabs::HudIcon::getIconSizeWorldUnits();
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
