#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/InputComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/generic/OpenableComponent.hpp"
#include "components/generic/ActivableComponent.hpp"
#include "components/generic/ClimbingComponent.hpp"

#include "system/InputSystem.hpp"
#include "EntityRegistry.hpp"
#include "Input.hpp"
#include "audio/Audio.hpp"
#include "Level.hpp"

#include <algorithm>

namespace
{
    constexpr float DEFAULT_DELTA_X = 0.01f;
    constexpr float DEFAULT_MAX_X_VELOCITY = 0.050f;
    constexpr float DEFAULT_MAX_Y_VELOCITY = 0.39f;
    constexpr float MAX_RUNNING_VELOCITY_X = 0.15f;
    constexpr float MAX_CRAWLING_VELOCITY_X = 0.008f;
    constexpr float CRAWLING_DELTA_X = 0.008f;
    constexpr float CLIMBING_VELOCITY_Y = 0.025f;
    constexpr float JUMP_SPEED = 0.165f;

    bool is_end_of_ladder(MapTile *tile)
    {
        if (tile->y - 1 >= 0)
        {
            auto* tile_above = Level::instance().get_tile_batch().map_tiles[tile->x][tile->y - 1];
            return !tile_above->climbable;
        }
        else
        {
            return true;
        }
    }

    entt::entity find_overlaping_items(entt::entity carrier_entity,
                                       PositionComponent& carrier_position,
                                       PhysicsComponent& carrier_physics,
                                       ItemCarrierComponent& carrier)
    {
        entt::entity out = entt::null;

        auto& registry = EntityRegistry::instance().get_registry();
        const auto items = registry.view<ItemComponent, PositionComponent, PhysicsComponent>();

        items.each([&](entt::entity item_entity,
                             ItemComponent& item,
                             PositionComponent& item_position,
                             PhysicsComponent& item_physics)
        {
            if (item_entity != carrier_entity &&
                !item.is_carried() &&
                carrier_physics.is_collision(item_physics, item_position, carrier_position))
            {
                out = item_entity;
            }
        });

        return out;
    }
}
void InputSystem::update(std::uint32_t delta_time_ms)
{
    _input_events = Input::instance().get_input_events();

    update_controllable_bodies();
    update_items_pick_up_put_down();
    update_items_open();
    update_items_activate();
}

void InputSystem::update_controllable_bodies()
{
    auto &registry = EntityRegistry::instance().get_registry();
    const auto controllable_bodies = registry.view<InputComponent, PhysicsComponent>();

    controllable_bodies.each([&](entt::entity entity, InputComponent &controllable_input, PhysicsComponent &controllable_physics)
    {
        for (const auto &allowed_event : controllable_input.allowed_events)
        {
            switch (allowed_event)
            {
                case InputEvent::RIGHT:
                {
                    if (consume(InputEvent::RIGHT))
                    {
                        if (controllable_physics.get_x_velocity() < 0.0f)
                        {
                            controllable_physics.set_x_velocity(0.0f);
                        }

                        controllable_physics.set_x_velocity(controllable_physics.get_x_velocity() + DEFAULT_DELTA_X);
                    }
                    continue;
                }
                case InputEvent::LEFT:
                {
                    if (consume(InputEvent::LEFT))
                    {
                        if (controllable_physics.get_x_velocity() > 0.0f)
                        {
                            controllable_physics.set_x_velocity(0.0f);
                        }

                        controllable_physics.set_x_velocity(controllable_physics.get_x_velocity() - DEFAULT_DELTA_X);
                    }
                    continue;
                }
                case InputEvent::RUNNING_FAST:
                {
                    if (consume(InputEvent::RUNNING_FAST))
                    {
                        controllable_physics.set_max_x_velocity(MAX_RUNNING_VELOCITY_X);
                    }
                    else
                    {
                        controllable_physics.set_max_x_velocity(DEFAULT_MAX_X_VELOCITY);
                    }
                    continue;
                }
                case InputEvent::JUMPING:
                case InputEvent::JUMPING_PRESSED:
                {
                    if (have_event(_input_events, InputEvent::JUMPING, InputEvent::JUMPING_PRESSED))
                    {
                        if (controllable_physics.is_bottom_collision())
                        {
                            consume(InputEvent::JUMPING, InputEvent::JUMPING_PRESSED);

                            auto total_jump_speed = JUMP_SPEED;

                            if (registry.has<ItemCarrierComponent>(entity))
                            {
                                total_jump_speed += registry.get<ItemCarrierComponent>(entity).get_modifiers().additional_jump_y_velocity;
                            }

                            controllable_physics.set_y_velocity(-total_jump_speed);
                        }
                    }
                    continue;
                }
                case InputEvent::UP:
                case InputEvent::DOWN:
                {
                    if (registry.has<ClimbingComponent>(entity))
                    {
                        auto& position = registry.get<PositionComponent>(entity);
                        auto* overlapped_tile = Level::instance().get_tile_batch().map_tiles[static_cast<int>(position.x_center)][static_cast<int>(position.y_center)];

                        if (overlapped_tile->climbable)
                        {
                            if (consume(allowed_event))
                            {
                                const float climbing_velocity = allowed_event == InputEvent::UP ? -0.04 : 0.04;

                                controllable_physics.set_y_velocity(climbing_velocity);
                                controllable_physics.set_x_velocity(0);
                                position.x_center = overlapped_tile->get_center_x();

                                auto& climbing_component = registry.get<ClimbingComponent>(entity);

                                if (allowed_event == InputEvent::UP && is_end_of_ladder(overlapped_tile))
                                {
                                    controllable_physics.set_y_velocity(0);
                                }

                                switch(overlapped_tile->map_tile_type)
                                {
                                    case MapTileType::LADDER:
                                    case MapTileType::LADDER_DECK:
                                    {
                                        climbing_component.notify({ClimbingEventType::STARTED_CLIMBING_LADDER});
                                        break;
                                    }
                                    default:
                                    {
                                        climbing_component.notify({ClimbingEventType::STARTED_CLIMBING_ROPE});
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    break;
                }
                default: continue;
            }
        }
    });
}

void InputSystem::update_items_pick_up_put_down()
{
    auto &registry = EntityRegistry::instance().get_registry();
    const auto item_carriers = registry.view<ItemCarrierComponent,
                                             PositionComponent,
                                             PhysicsComponent,
                                             InputComponent,
                                             HorizontalOrientationComponent>();

    item_carriers.each([&](entt::entity carrier_entity,
                                ItemCarrierComponent& carrier,
                                PositionComponent& carrier_position,
                                PhysicsComponent& carrier_physics,
                                InputComponent& carrier_input,
                                HorizontalOrientationComponent& carrier_orientation)
    {
        const bool pick_up_or_put_down_intent = have_event(_input_events, InputEvent::DUCKING, InputEvent::THROWING_PRESSED);
        const bool open_intent = have_event(_input_events, InputEvent::UP, InputEvent::THROWING_PRESSED);
        const bool throw_intent = have_event(_input_events, InputEvent::THROWING_PRESSED);

        if (pick_up_or_put_down_intent)
        {
            if (carrier.has_active_item())
            {
                carrier.put_down_active_item();
                consume(InputEvent::DUCKING, InputEvent::THROWING_PRESSED);
            }
            else
            {
                auto entity = find_overlaping_items(carrier_entity, carrier_position, carrier_physics, carrier);
                if (entity != entt::null)
                {
                    carrier.pick_up_item(entity, carrier_entity);
                    consume(InputEvent::DUCKING, InputEvent::THROWING_PRESSED);
                }
            }
        }
        else if (carrier.has_active_item())
        {
            auto& item = carrier.get_active_item();

            if (!open_intent && throw_intent && item.get_application() != ItemApplication::ACTIVABLE)
            {
                carrier.throw_active_item(carrier_orientation, carrier_physics);
                consume(InputEvent::THROWING_PRESSED);
                Audio::instance().play(SFXType::THROW);
            }
        }
    });
}

void InputSystem::update_items_activate()
{
    auto &registry = EntityRegistry::instance().get_registry();
    const auto activables = registry.view<ActivableComponent, ItemComponent>();

    activables.each([&](ActivableComponent &activable, ItemComponent& activable_item)
    {
        if (!activable_item.is_carried() || !registry.has<InputComponent>(activable_item.get_item_carrier_entity()))
        {
            return;
        }

        const auto& carrier_input_component = registry.get<InputComponent>(activable_item.get_item_carrier_entity());

        activable.activated = true;
        std::vector<InputEvent> events_to_consume;

        for (const auto& event : activable.activate_combination)
        {
            if (have_event(_input_events, event) &&
                have_event(carrier_input_component.allowed_events, event))
            {
                events_to_consume.push_back(event);
            }
            else
            {
                activable.activated = false;
                break;
            }
        }

        if (activable.activated)
        {
            for (auto& event : events_to_consume)
            {
                consume(event);
            }
        }
    });
}

void InputSystem::update_items_open()
{
    if (!have_event(_input_events, InputEvent::UP, InputEvent::THROWING_PRESSED))
    {
        return;
    }

    auto& registry = EntityRegistry::instance().get_registry();
    const auto openable_items = registry.view<OpenableComponent, PositionComponent, PhysicsComponent>();
    const auto entities_that_can_open = registry.view<InputComponent, PositionComponent, PhysicsComponent>();

    entities_that_can_open.each([&](
            entt::entity can_open_entity,
            InputComponent& can_open_input,
            PositionComponent& can_open_position,
            PhysicsComponent& can_open_physics)
    {
        if (!have_event(can_open_input.allowed_events, InputEvent::UP, InputEvent::THROWING_PRESSED))
        {
            return;
        }

        openable_items.each([&](entt::entity openable_entity,
                                     OpenableComponent& openable,
                                     PositionComponent& openable_position,
                                     PhysicsComponent& openable_physics)
        {
            if (can_open_physics.is_collision(openable_physics, openable_position, can_open_position))
            {
                openable.opened = true;
                openable.who_opened = can_open_entity;

                consume(InputEvent::UP, InputEvent::THROWING_PRESSED);

                if (registry.has<ItemComponent>(openable_entity))
                {
                    auto& item = registry.get<ItemComponent>(openable_entity);
                    item.set_type(ItemApplication::THROWABLE);
                }

                return;
            }
        });
    });
}

template <class InputEvent>
bool InputSystem::have_event(const std::vector<InputEvent>& events, InputEvent event)
{
    return std::find(events.begin(), events.end(), event) != events.end();
}

template <typename InputEvent, typename... Args>
bool InputSystem::have_event(const std::vector<InputEvent>& events, InputEvent first, Args... args)
{
    if (!have_event(events, first))
    {
        return false;
    }
    else
    {
        return have_event(events, args...);
    }
}

template <class InputEvent>
bool InputSystem::consume(InputEvent arg)
{
    const auto iter = std::remove(_input_events.begin(), _input_events.end(), arg);

    if (iter == _input_events.end())
    {
        return false;
    }

    _input_events.erase(iter);

    return true;
}

template <typename InputEvent, typename... Args>
bool InputSystem::consume(InputEvent first, Args... args)
{
    if (!consume(first))
    {
        return false;
    }
    else
    {
        return consume(args...);
    }
}
