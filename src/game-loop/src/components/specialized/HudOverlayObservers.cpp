#include "components/specialized/HudOverlayObservers.hpp"
#include "components/specialized/HudOverlayComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
#include "components/generic/SaleableComponent.hpp"
#include "prefabs/ui/HudIcon.hpp"
#include "viewport/Viewport.hpp"

void HudOverlayItemObserver::on_notify(const ItemCarrierEvent * event)
{
    assert(_viewport);

    auto& registry = EntityRegistry::instance().get_registry();
    if (registry.has<SaleableComponent>(event->item))
    {
        return;
    }

    const auto new_icon_position = get_item_icon_position(_displayed_items);

    switch (event->event_type)
    {
        case ItemCarrierEvent::EventType::ADDED:
        {
            if (displays_item(event->item_type))
            {
                return;
            }

            entt::entity entity;

            switch (event->item_type)
            {
                case ItemType::SPIKE_SHOES: entity = prefabs::HudIcon::create(new_icon_position.x, new_icon_position.y, HUDSpritesheetFrames::SPIKE_SHOES_ICON); break;
                case ItemType::SPRING_SHOES: entity = prefabs::HudIcon::create(new_icon_position.x, new_icon_position.y, HUDSpritesheetFrames::SPRING_SHOES_ICON); break;
                case ItemType::MITT: entity = prefabs::HudIcon::create(new_icon_position.x, new_icon_position.y, HUDSpritesheetFrames::MITT_ICON); break;
                case ItemType::GLOVE: entity = prefabs::HudIcon::create(new_icon_position.x, new_icon_position.y, HUDSpritesheetFrames::GLOVES_ICON); break;
                case ItemType::JETPACK: entity = prefabs::HudIcon::create(new_icon_position.x, new_icon_position.y, HUDSpritesheetFrames::JETPACK_ICON); break;
                case ItemType::CAPE: entity = prefabs::HudIcon::create(new_icon_position.x, new_icon_position.y, HUDSpritesheetFrames::CAPE_ICON); break;
                case ItemType::COMPASS:
                {
                    entity = prefabs::HudIcon::create(new_icon_position.x, new_icon_position.y, HUDSpritesheetFrames::COMPASS_ICON);
                    // Subscribe on compass events:
                    const auto& compass_entity = event->item;
                    auto& compass_scripting_component = registry.get<ScriptingComponent>(compass_entity);
                    auto* compass_script = reinterpret_cast<prefabs::CompassScript*>(compass_scripting_component.script.get());
                    compass_script->add_observer(reinterpret_cast<Observer<prefabs::CompassArrow>*>(_compass_arrow_observer.get()));
                    break;
                }
                default:
                {
                    // Don't display anything in case of any other items
                    return;
                }
            }

            _children.push_back({event->item_type, entity});
            _displayed_items++;

            std::sort(_children.begin(), _children.end(), [](const auto& lhs, const auto& rhs)
            {
                return static_cast<int>(lhs.first) > static_cast<int>(rhs.second);
            });

            break;
        }
        case ItemCarrierEvent::EventType::REMOVED:
        {
            if (!displays_item(event->item_type))
            {
                return;
            }

            const auto iter = std::remove_if(_children.begin(), _children.end(), [event, &registry](const auto type_entity_pair)
            {
                const auto& icon_entity = type_entity_pair.second;
                if (event->item_type == type_entity_pair.first)
                {
                    registry.destroy(icon_entity);
                    return true;
                }
                else
                {
                    return false;
                }
            });

            assert(iter != _children.end());
            _children.erase(iter);
            _displayed_items--;

            // Re-calculate positions for each existing icon:

            std::size_t index = 0;

            for (auto& type_entity_pair : _children)
            {
                const auto pos_recalculated = get_item_icon_position(index);

                auto& entity = type_entity_pair.second;
                auto& position_component = registry.get<PositionComponent>(entity);
                position_component.x_center = pos_recalculated.x;
                position_component.y_center = pos_recalculated.y;

                index++;
            }

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
    _compass_arrow_observer = std::make_shared<HudCompassArrowObserver>(_viewport);
}

bool HudOverlayItemObserver::displays_item(ItemType type) const
{
    return std::any_of(_children.begin(), _children.end(), [type](const auto& item_entity_pair)
    {
        return item_entity_pair.first == type;
    });
}

Point2D HudOverlayItemObserver::get_item_icon_position(std::size_t index) const
{
    const auto pos_x = static_cast<float>(_viewport->get_width_world_units() * 0.05f) + ((index + 1) * ICONS_OFFSET_WORLD_UNITS * 0.5f);
    const auto pos_y = static_cast<float>(_viewport->get_height_world_units() * 0.05f) + ICONS_OFFSET_WORLD_UNITS * 0.5f;
    return {pos_x, pos_y};
}

void HudCompassArrowObserver::on_notify(const prefabs::CompassArrow* arrow)
{
    auto& registry = EntityRegistry::instance().get_registry();

    if (_arrow != entt::null)
    {
        registry.destroy(_arrow);
        _arrow = entt::null;
    }

    switch (*arrow)
    {
        case prefabs::CompassArrow::NONE: break;
        case prefabs::CompassArrow::LEFT:
        {
            const float pos_x = 0 + prefabs::HudIcon::getIconSizeWorldUnits();
            const float pos_y = _viewport->get_height_world_units() / 2;
            _arrow = prefabs::HudIcon::create(pos_x, pos_y, HUDSpritesheetFrames::COMPASS_ARROW_LEFT);
            break;
        }
        case prefabs::CompassArrow::RIGHT:
        {
            const float pos_x = _viewport->get_width_world_units() - prefabs::HudIcon::getIconSizeWorldUnits();
            const float pos_y = _viewport->get_height_world_units() / 2;
            _arrow = prefabs::HudIcon::create(pos_x, pos_y, HUDSpritesheetFrames::COMPASS_ARROW_RIGHT);
            break;
        }
        case prefabs::CompassArrow::DOWN:
        {
            const float pos_x = _viewport->get_width_world_units() / 2;
            const float pos_y = _viewport->get_height_world_units() - prefabs::HudIcon::getIconSizeWorldUnits();
            _arrow = prefabs::HudIcon::create(pos_x, pos_y, HUDSpritesheetFrames::COMPASS_ARROW_DOWN);
            break;
        }
        case prefabs::CompassArrow::LEFT_BOTTOM:
        {
            const float pos_x = 0 + prefabs::HudIcon::getIconSizeWorldUnits();
            const float pos_y = _viewport->get_height_world_units() - prefabs::HudIcon::getIconSizeWorldUnits();
            _arrow = prefabs::HudIcon::create(pos_x, pos_y, HUDSpritesheetFrames::COMPASS_ARROW_LEFT_BOTTOM);
            break;
        }
        case prefabs::CompassArrow::RIGHT_BOTTOM:
        {
            const float pos_x = _viewport->get_width_world_units() - prefabs::HudIcon::getIconSizeWorldUnits();
            const float pos_y = _viewport->get_height_world_units() - prefabs::HudIcon::getIconSizeWorldUnits();
            _arrow = prefabs::HudIcon::create(pos_x, pos_y, HUDSpritesheetFrames::COMPASS_ARROW_RIGHT_BOTTOM);
            break;
        }
        default: assert(false);
    }
}

HudCompassArrowObserver::~HudCompassArrowObserver()
{
    auto& registry = EntityRegistry::instance().get_registry();

    if (_arrow != entt::null)
    {
        registry.destroy(_arrow);
    }
}

HudCompassArrowObserver::HudCompassArrowObserver(std::shared_ptr<Viewport> viewport) : _viewport(std::move(viewport))
{
}
