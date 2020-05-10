#include "components/PhysicsComponent.hpp"
#include "MainDude.hpp"
#include "LevelGenerator.hpp"
#include "Collisions.hpp"

#include <cmath>
#include <logger/log.h>
#include <components/PhysicsComponent.hpp>

namespace
{
    constexpr float smallest_position_step = 1.0f / 16.0f;

    constexpr double default_max_x_speed = 0.1875f;
    constexpr double default_max_y_speed = 0.21875f;
    constexpr double default_friction = 0.017f;
    constexpr float default_gravity = 0.011875f;
    constexpr float default_bouncing_factor_x = 0.15f;
    constexpr float default_bouncing_factor_y = 0.35f;
    constexpr uint16_t default_pos_update_delta_ms = 15;

    float move_to_zero(float value, float amount)
    {
        if (value == 0)
        {
            return 0.0f;
        }

        if (value > 0)
        {
            if (value - amount < 0)
            {
                return 0;
            } else
            {
                return value - amount;
            }
        } else
        {
            if (value + amount > 0)
            {
                return 0;
            } else
            {
                return value + amount;
            }
        }
    }
}

void PhysicsComponent::update(MainDude &main_dude, uint32_t delta_time_ms)
{
    _pos_update_delta_ms += delta_time_ms;

    while (_pos_update_delta_ms >= default_pos_update_delta_ms)
    {
        _pos_update_delta_ms -= default_pos_update_delta_ms;

        // Step by step:

        MapTile* neighbours[9] = { nullptr };

        float last_step_x = main_dude._x;
        float last_step_y = main_dude._y;

        float temp_velocity_x = _velocity.x;
        float temp_velocity_y = _velocity.y;

        _collisions.left = false;
        _collisions.right = false;
        _collisions.upper = false;
        _collisions.bottom = false;

        while (temp_velocity_x != 0.0f || temp_velocity_y != 0.0f)
        {
            if (temp_velocity_x != 0.0f)
            {
                // Step on X axis

                main_dude._x += std::copysign(smallest_position_step, temp_velocity_x);
                collisions::get_neighbouring_tiles(LevelGenerator::instance().getLevel(), main_dude._x, main_dude._y, neighbours);
                const auto* overlapping_tile = collisions::overlaps(neighbours, main_dude._x, main_dude._y, _dimensions.width, _dimensions.height);
                if (overlapping_tile)
                {
                    // step back
                    main_dude._x = last_step_x;

                    if (_velocity.x < 0.0f)
                    {
                        _collisions.left = true;
                    }
                    else
                    {
                        _collisions.right = true;
                    }

                    _velocity.x = 0.0f;
                    temp_velocity_x = 0.0f;
                }
                else
                {
                    last_step_x = main_dude._x;
                    temp_velocity_x = move_to_zero(temp_velocity_x, smallest_position_step);
                }
            }

            if (temp_velocity_y != 0.0f)
            {
                // Step on Y axis

                main_dude._y += std::copysign(smallest_position_step, temp_velocity_y);
                collisions::get_neighbouring_tiles(LevelGenerator::instance().getLevel(), main_dude._x, main_dude._y, neighbours);
                const auto* overlapping_tile = collisions::overlaps(neighbours, main_dude._x, main_dude._y, _dimensions.width, _dimensions.height);
                if (overlapping_tile)
                {
                    // step back
                    main_dude._y = last_step_y;

                    if (_velocity.y < 0.0f)
                    {
                        _collisions.upper = true;
                    }
                    else
                    {
                        _collisions.bottom = true;
                    }

                    _velocity.y = 0.0f;
                    temp_velocity_y = 0.0f;
                }
                else
                {
                    last_step_y = main_dude._y;
                    temp_velocity_y = move_to_zero(temp_velocity_y, smallest_position_step);
                }
            }
        }

        if (_collisions.bottom)
        {
            // Apply friction
            _velocity.x += std::copysign(default_friction, -_velocity.x);

            // For near-zero speed make it zero:
            if (std::abs(_velocity.x) <= default_friction)
            {
                _velocity.x = 0;
            }
        }
        else
        {
            // Apply gravity
            _velocity.y += default_gravity;
        }

        // Limit speed

        if (std::abs(_velocity.x) > default_max_x_speed)
        {
            _velocity.x = std::copysign(default_max_x_speed, _velocity.x);
        }

        if (std::abs(_velocity.y) > default_max_y_speed)
        {
            _velocity.y = std::copysign(default_max_y_speed, _velocity.y);
        }
    }
}

PhysicsComponent::PhysicsComponent(float width, float height) : _dimensions{width, height}
{

}

void PhysicsComponent::add_velocity(float x, float y)
{
    _velocity.x += x;
    _velocity.y += y;
}
