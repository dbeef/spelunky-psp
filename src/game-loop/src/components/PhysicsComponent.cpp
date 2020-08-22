#include "components/PhysicsComponent.hpp"
#include "main-dude/MainDude.hpp"
#include "Level.hpp"
#include "Collisions.hpp"

// Using C-style <math.h> instead of <cmath> because of some symbols (namely std::copysign)
// are missing in the PSP's CPP standard library.
#include <math.h>
#include <components/PhysicsComponent.hpp>

namespace
{
    constexpr float SMALLEST_POSITION_STEP = 1.0f / 64.0f; // TODO: Should be a function of velocity
    constexpr float BOUNCE_EPSILON = 0.05f;
    constexpr float GRAVITY = 0.011f;
    constexpr uint16_t POS_UPDATE_DELTA_MS = 15;

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
            }
            else
            {
                return value - amount;
            }
        }
        else
        {
            if (value + amount > 0)
            {
                return 0;
            }
            else
            {
                return value + amount;
            }
        }
    }
}

void PhysicsComponent::update(uint32_t delta_time_ms)
{
    _pos_update_delta_ms += delta_time_ms;

    // Limit speed

    if (std::abs(_velocity.x) > _velocity.max_x)
    {
        _velocity.x = copysign(_velocity.max_x, _velocity.x);
    }

    if (std::abs(_velocity.y) > _velocity.max_y)
    {
        _velocity.y = copysign(_velocity.max_y, _velocity.y);
    }

    bool initial_check_done = false;

    while (_pos_update_delta_ms >= POS_UPDATE_DELTA_MS)
    {
        _pos_update_delta_ms -= POS_UPDATE_DELTA_MS;

        // Initial check:

        if (!initial_check_done)
        {
            MapTile *neighbours[9] = {nullptr};
            Level::instance().get_tile_batch().get_neighbouring_tiles(_position.x, _position.y, neighbours);
            const auto *overlapping_tile = collisions::overlaps_strict(neighbours, _position.x, _position.y, _dimensions.width, _dimensions.height);
            if (overlapping_tile)
            {
                if (_velocity.x < 0.0f)
                {
                    _collisions.left = true;
                }
                else if (_velocity.x > 0.0f)
                {
                    _collisions.right = true;
                }
                else
                {
                    _collisions.right = false;
                    _collisions.left = false;
                }

                if (_velocity.y < 0.0f)
                {
                    _collisions.upper = true;
                }
                else if (_velocity.y > 0.f)
                {
                    _collisions.bottom = true;
                }
            }
            else
            {
                _collisions.right = false;
                _collisions.left = false;

                _collisions.upper = false;
                _collisions.bottom = false;
            }

            initial_check_done = true;
        }

        // Step by step:

        MapTile* neighbours[9] = { nullptr };

        float temp_velocity_x = _velocity.x;
        float temp_velocity_y = _velocity.y;

        while (temp_velocity_x != 0.0f || temp_velocity_y != 0.0f)
        {
            if (temp_velocity_x != 0.0f)
            {
                // Step on X axis

                _position.x += copysign(SMALLEST_POSITION_STEP, temp_velocity_x);
                Level::instance().get_tile_batch().get_neighbouring_tiles(_position.x, _position.y, neighbours);
                const auto* overlapping_tile = collisions::overlaps
                        (neighbours, _position.x, _position.y, _dimensions.width, _dimensions.height);
                if (overlapping_tile)
                {
                    // step back
                    if (_velocity.x < 0.0f)
                    {
                        _collisions.left = true;
                        _position.x = overlapping_tile->x + MapTile::PHYSICAL_WIDTH + (get_width() / 2.0f);
                    }
                    else
                    {
                        _position.x = overlapping_tile->x - (get_width() / 2.0f);
                        _collisions.right = true;
                    }

                    _velocity.x = -1 * _properties.bounciness * _velocity.x;
                    if (std::abs(_velocity.x) < BOUNCE_EPSILON)
                    {
                        _velocity.x = 0.0f;
                    }
                    temp_velocity_x = _velocity.x;
                }
                else
                {
                    _collisions.right = false;
                    _collisions.left = false;
                    temp_velocity_x = move_to_zero(temp_velocity_x, SMALLEST_POSITION_STEP);
                }
            }

            if (temp_velocity_y != 0.0f)
            {
                // Step on Y axis

                _position.y += copysign(SMALLEST_POSITION_STEP, temp_velocity_y);
                Level::instance().get_tile_batch().get_neighbouring_tiles(_position.x,  _position.y, neighbours);
                const auto* overlapping_tile = collisions::overlaps(neighbours, _position.x, _position.y, _dimensions.width, _dimensions.height);
                if (overlapping_tile)
                {
                    // step back

                    if (_velocity.y < 0.0f)
                    {
                        _collisions.upper = true;
                        _position.y = overlapping_tile->y + MapTile::PHYSICAL_HEIGHT + (get_height() / 2);
                    }
                    else
                    {
                        _collisions.bottom = true;
                        _position.y = overlapping_tile->y - get_height() / 2;
                    }

                    _velocity.y = -1 * _properties.bounciness * _velocity.y;
                    if (std::abs(_velocity.y) < BOUNCE_EPSILON)
                    {
                        _velocity.y = 0.0f;
                    }

                    temp_velocity_y = _velocity.y;
                }
                else
                {
                    _collisions.upper = false;
                    _collisions.bottom = false;
                    temp_velocity_y = move_to_zero(temp_velocity_y, SMALLEST_POSITION_STEP);
                }
            }
        }

        if (_collisions.bottom) // Apply friction
        {
            _velocity.x = move_to_zero(_velocity.x, _properties.friction);
        }
        else if (_gravity) // Apply gravity
        {
            _velocity.y += GRAVITY;
        }
    }
}

PhysicsComponent::PhysicsComponent(float width, float height) : _dimensions{width, height}
{

}
