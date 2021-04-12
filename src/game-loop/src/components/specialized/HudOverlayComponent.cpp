#include <utility>
#include <sstream>
#include <string>
#include <algorithm>

#include "EntityRegistry.hpp"
#include "prefabs/ui/Text.hpp"
#include "prefabs/ui/HudIcon.hpp"

#include "components/generic/PositionComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/specialized/HudOverlayComponent.hpp"
#include "spritesheet-frames/HUDSpritesheetFrames.hpp"
#include "other/Inventory.hpp"
#include "prefabs/items/SpikeShoes.hpp"
#include "prefabs/items/SpringShoes.hpp"
#include "prefabs/items/Mitt.hpp"
#include "prefabs/items/Glove.hpp"

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

void HudOverlayItemObserver::on_notify(const ItemCarrierEvent * event)
{
    // TODO: Should re-create the icons upon entering a new level

    assert(_viewport);

    auto& registry = EntityRegistry::instance().get_registry();

    const auto pos_x = static_cast<float>(_viewport->get_width_world_units() * 0.05f) + ((_displayed_items + 1) * ICONS_OFFSET_WORLD_UNITS * 0.5f);
    const auto pos_y = static_cast<float>(_viewport->get_height_world_units() * 0.05f) + ICONS_OFFSET_WORLD_UNITS * 0.5f;

    switch (event->event_type)
    {
        case ItemCarrierEvent::EventType::EQUIPPED:
        {
            if (displays_item(event->item_type))
            {
                return;
            }

            entt::entity entity = entt::null;

            switch (event->item_type)
            {
                // TODO: Icons for jetpack and cape are different than their item quads - add them and display here
                //       What's more, most of items have separate images for icons. How to address this?
                //       Just a new prefab, like SomethingIcon, i.e JetpackIcon, MittIcon...? If so, how to differentiate,
                //       just by a switch case here match a prefab with item type?
                case ItemType::SPIKE_SHOES: entity = prefabs::SpikeShoes::create(pos_x, pos_y); break;
                case ItemType::SPRING_SHOES: entity = prefabs::SpringShoes::create(pos_x, pos_y); break;
                case ItemType::MITT: entity = prefabs::Mitt::create(pos_x, pos_y); break;
                case ItemType::GLOVE: entity = prefabs::Glove::create(pos_x, pos_y); break;
                default:
                {
                    // Don't display anything in case of any other items
                    return;
                }
            }

            // Remove needless components:
            registry.remove<ScriptingComponent>(entity);
            registry.remove<ItemComponent>(entity);
            registry.remove<PhysicsComponent>(entity);

            // Edit the mesh component to display in screen-space instead of model-view space:
            auto& mesh = registry.get<MeshComponent>(entity);
            mesh.camera_type = CameraType::SCREEN_SPACE;
            mesh.rendering_layer = RenderingLayer::LAYER_2_HUD;

            // Slightly change the scale:
            auto& quad = registry.get<QuadComponent>(entity);
            quad.set_quad_width(quad.get_quad_width() * 0.75f);
            quad.set_quad_height(quad.get_quad_height() * 0.75f);

            _children.push_back({ItemType::SPIKE_SHOES, entity});
            _displayed_items++;

            break;
        }
        case ItemCarrierEvent::EventType::DROPPED: // TODO: Rename to "removed"
        {
            if (!displays_item(event->item_type))
            {
                return;
            }

            _displayed_items--;

            // TODO: Hide icon of removed item
            break;
        }
    }
}

HudOverlayItemObserver::~HudOverlayItemObserver()
{
    auto& registry = EntityRegistry::instance().get_registry();
    for (const auto& type_entity_pair : _children)
    {
        auto& entity = type_entity_pair.second;

        if (registry.valid(entity))
        {
            registry.destroy(entity);
        }
    }
}

HudOverlayItemObserver::HudOverlayItemObserver(std::shared_ptr<Viewport> viewport) : _viewport(std::move(viewport))
{
}

bool HudOverlayItemObserver::displays_item(ItemType type) const
{
    return std::any_of(_children.begin(), _children.end(), [type](const auto& item_entity_pair)
    {
        return item_entity_pair.first == type;
    });
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
        _texts.bombs = prefabs::Text::create(x, y, to_string(Inventory::instance().get_bombs()));
    }

    {
        const float x = BASE_POS_X + (ICONS_OFFSET_WORLD_UNITS * 2) + prefabs::HudIcon::getIconSizeWorldUnits();
        const float y = BASE_POS_Y - (prefabs::HudIcon::getIconSizeWorldUnits() * 0.425);
        _texts.ropes = prefabs::Text::create(x, y, to_string(Inventory::instance().get_ropes()));
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
