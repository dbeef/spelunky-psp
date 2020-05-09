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
    constexpr double default_friction = 0.0034375f;
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

        float delta_v_x = _velocity.x;
        float delta_v_y = _velocity.y;

        _collisions.bottom = false;
        _collisions.upper = false;
        _collisions.left = false;
        _collisions.right = false;

        while (delta_v_x != 0 || delta_v_y != 0)
        {
            MapTile *neighbours[9] = { nullptr };
            collisions::get_neighbouring_tiles(LevelGenerator::instance().getLevel(), main_dude._x, main_dude._y, neighbours);

            // Update velocity step-by-step:
            if (delta_v_x != 0)
            {
                main_dude._x += std::copysign(smallest_position_step, delta_v_x);
                delta_v_x = move_to_zero(delta_v_x, smallest_position_step);
                auto collision_tile_left = collisions::check_left_collision(neighbours, main_dude._x, main_dude._y, _dimensions.width, _dimensions.height);
                if (collision_tile_left)
                {
                    main_dude._x = collision_tile_left->x + 1.0f + (_dimensions.width / 2.0f); // TODO: Global const literal for tile width/height
                    _collisions.left = true;
                }

                auto collision_tile_right = collisions::check_right_collision(neighbours, main_dude._x, main_dude._y, _dimensions.width, _dimensions.height);
                if (collision_tile_right)
                {
                    main_dude._x = collision_tile_right->x - (_dimensions.width / 2.0f);
                    _collisions.right = true;
                }
            }
            else
            {
                main_dude._y += std::copysign(smallest_position_step, delta_v_y);
                delta_v_y = move_to_zero(delta_v_y, smallest_position_step);
                auto collision_tile_bottom = collisions::check_bottom_collision(neighbours, main_dude._x, main_dude._y, _dimensions.width, _dimensions.height);
                if (collision_tile_bottom)
                {
                    main_dude._y = collision_tile_bottom->y - (_dimensions.height / 2.0f);
                    _collisions.bottom = true;
                }

                auto collision_tile_top = collisions::check_top_collision(neighbours, main_dude._x, main_dude._y, _dimensions.width, _dimensions.height);
                if (collision_tile_top)
                {
                    main_dude._y = collision_tile_top->y + 1.0f + (_dimensions.height / 2.0f);
                    _collisions.upper = true;
                }
            }
        }

        if (_collisions.left || _collisions.right)
        {
            _velocity.x = 0;
        }


        if (_collisions.bottom || _collisions.upper)
        {
            _velocity.y = 0;
        }

        if (_collisions.bottom)
        {
            // Apply friction
            if (_velocity.x > 0)
            {
                _velocity.x -= default_friction;
            }
            if (_velocity.x < 0)
            {
                _velocity.x += default_friction;
            }
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
            if (_velocity.x > 0)
            {
                _velocity.x = default_max_x_speed;
            } else
            {
                _velocity.x = -default_max_x_speed;
            }
        }

        if (std::abs(_velocity.y) > default_max_y_speed)
        {
            if (_velocity.y > 0)
            {
                _velocity.y = default_max_y_speed;
            } else
            {
                _velocity.y = -default_max_y_speed;
            }
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
