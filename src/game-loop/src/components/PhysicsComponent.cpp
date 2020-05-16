#include "components/PhysicsComponent.hpp"
#include "main-dude/MainDude.hpp"
#include "LevelGenerator.hpp"
#include "Collisions.hpp"

// Using C-style <math.h> instead of <cmath> because of some symbols (namely std::copysign)
// being missing in the PSP's CPP standard library.
#include <math.h>

namespace
{
    // TODO: Should be a function of velocity
    constexpr float smallest_position_step = 1.0f / 128.0f;

    constexpr float default_gravity = 0.011875f;
    constexpr float default_friction = 0.005f;
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

        float temp_velocity_x = _velocity.x;
        float temp_velocity_y = _velocity.y;

        _collisions.left = false;
        _collisions.right = false;
        _collisions.upper = false;
        _collisions.bottom = false;

        {
            collisions::get_neighbouring_tiles(LevelGenerator::instance().getLevel(), _position.x, _position.y, neighbours);
            const auto* overlapping_tile = collisions::overlaps_strict(neighbours, _position.x, _position.y, _dimensions.width, _dimensions.height);
            if (overlapping_tile)
            {
                if (_velocity.x < 0.0f)
                {
                    _collisions.left = true;
                }
                else
                {
                    _collisions.right = true;
                }
                if (_velocity.y < 0.0f)
                {
                    _collisions.upper = true;
                }
                else
                {
                    _collisions.bottom = true;
                }
            }
        }

        while (temp_velocity_x != 0.0f || temp_velocity_y != 0.0f)
        {
            if (temp_velocity_x != 0.0f)
            {
                // Step on X axis

                _position.x += copysign(smallest_position_step, temp_velocity_x);
                collisions::get_neighbouring_tiles(LevelGenerator::instance().getLevel(), _position.x, _position.y, neighbours);
                const auto* overlapping_tile = collisions::overlaps(neighbours, _position.x, _position.y, _dimensions.width, _dimensions.height);
                if (overlapping_tile)
                {
                    // step back
                    if (_velocity.x < 0.0f)
                    {
                        _collisions.left = true;
                        _position.x = overlapping_tile->x + 1.0f + (get_width() / 2.0f);
                    }
                    else
                    {
                        _position.x = overlapping_tile->x - (get_width() / 2.0f);
                        _collisions.right = true;
                    }

                    _velocity.x = 0.0f;
                    temp_velocity_x = 0.0f;
                }
                else
                {
                    temp_velocity_x = move_to_zero(temp_velocity_x, smallest_position_step);
                }
            }

            if (temp_velocity_y != 0.0f)
            {
                // Step on Y axis

                _position.y += copysign(smallest_position_step, temp_velocity_y);
                collisions::get_neighbouring_tiles(LevelGenerator::instance().getLevel(), _position.x,  _position.y, neighbours);
                const auto* overlapping_tile = collisions::overlaps(neighbours, _position.x, _position.y, _dimensions.width, _dimensions.height);
                if (overlapping_tile)
                {
                    // step back

                    if (_velocity.y < 0.0f)
                    {
                        _collisions.upper = true;
                        _position.y = overlapping_tile->y + 1.0f + (get_width() / 2);
                    }
                    else
                    {
                        _collisions.bottom = true;
                        _position.y = overlapping_tile->y - 1.0f + (get_width() / 2);
                    }

                    _velocity.y = 0.0f;
                    temp_velocity_y = 0.0f;
                }
                else
                {
                    temp_velocity_y = move_to_zero(temp_velocity_y, smallest_position_step);
                }
            }
        }

        if (_collisions.bottom)
        {
            // Apply friction
            _velocity.x = move_to_zero(_velocity.x, default_friction);
        }
        else
        {
            // Apply gravity
            _velocity.y += default_gravity;
        }

        // Limit speed

        if (std::abs(_velocity.x) > _velocity.max_x)
        {
            _velocity.x = copysign(_velocity.max_x, _velocity.x);
        }

        if (std::abs(_velocity.y) > _velocity.max_y)
        {
            _velocity.y = copysign(_velocity.max_y, _velocity.y);
        }
    }
}

PhysicsComponent::PhysicsComponent(float width, float height) : _dimensions{width, height}
{

}

