#include "components/generic/PhysicsComponent.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "Level.hpp"
#include "Collisions.hpp"

// Using C-style <math.h> instead of <cmath> because of some symbols (namely std::copysign)
// are missing in the PSP's CPP standard library.
#include <math.h>
#include <logger/log.h>

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

void PhysicsComponent::update(uint32_t delta_time_ms, PositionComponent& position)
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
            Level::instance().get_tile_batch().get_neighbouring_tiles(position.x_center, position.y_center, neighbours);
            const auto *overlapping_tile = collisions::overlaps_strict(neighbours, position.x_center, position.y_center, _dimensions.width, _dimensions.height);
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

                position.x_center += copysign(SMALLEST_POSITION_STEP, temp_velocity_x);
                // FIXME: This is to prevent moving off-map. Think of a better solution.
                position.x_center = std::clamp(position.x_center, 0.75f, 31.25f);

                Level::instance().get_tile_batch().get_neighbouring_tiles(position.x_center, position.y_center, neighbours);
                const auto* overlapping_tile = collisions::overlaps
                        (neighbours, position.x_center, position.y_center, _dimensions.width, _dimensions.height);
                if (overlapping_tile)
                {
                    // step back
                    if (_velocity.x < 0.0f)
                    {
                        _collisions.left = true;
                        position.x_center = overlapping_tile->x + MapTile::PHYSICAL_WIDTH + (get_width() / 2.0f);
                        // FIXME: This is to prevent moving off-map. Think of a better solution.
                        position.x_center = std::clamp(position.x_center, 0.75f, 31.25f);
                    }
                    else
                    {
                        position.x_center = overlapping_tile->x - (get_width() / 2.0f);
                        _collisions.right = true;
                        // FIXME: This is to prevent moving off-map. Think of a better solution.
                        position.x_center = std::clamp(position.x_center, 0.75f, 31.25f);
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

                position.y_center += copysign(SMALLEST_POSITION_STEP, temp_velocity_y);
                // FIXME: This is to prevent moving off-map. Think of a better solution.
                position.y_center = std::clamp(position.y_center, 0.75f, 31.25f);
                Level::instance().get_tile_batch().get_neighbouring_tiles(position.x_center,  position.y_center, neighbours);
                const auto* overlapping_tile = collisions::overlaps(neighbours, position.x_center, position.y_center, _dimensions.width, _dimensions.height);
                if (overlapping_tile)
                {
                    // step back

                    if (_velocity.y < 0.0f)
                    {
                        _collisions.upper = true;
                        position.y_center = overlapping_tile->y + MapTile::PHYSICAL_HEIGHT + (get_height() / 2);
                        // FIXME: This is to prevent moving off-map. Think of a better solution.
                        position.y_center = std::clamp(position.y_center, 0.75f, 31.25f);
                        assert(position.y_center >= 0.0f && position.y_center <= 32.0f);
                    }
                    else
                    {
                        _collisions.bottom = true;
                        position.y_center = overlapping_tile->y - get_height() / 2;
                        // FIXME: This is to prevent moving off-map. Think of a better solution.
                        position.y_center = std::clamp(position.y_center, 0.75f, 31.25f);
                        assert(position.y_center >= 0.0f && position.y_center <= 32.0f);
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
            _velocity.y += GRAVITY * _properties.gravity_modifier;
        }
    }
}

PhysicsComponent::PhysicsComponent(float width, float height) : _dimensions{width, height}
{
}

bool PhysicsComponent::is_collision(ZoneComponent& other_zone, PositionComponent& other_position, PositionComponent& this_position) const
{
    const float half_w = _dimensions.width / 2.0f;
    const float half_h = _dimensions.height / 2.0f;

    const float other_half_w = other_zone.width / 2.0f;
    const float other_half_h = other_zone.height / 2.0f;

    return this_position.x_center + half_w > other_position.x_center - other_half_w && this_position.x_center - half_w < other_position.x_center + other_half_w &&
           this_position.y_center + half_h > other_position.y_center - other_half_h && this_position.y_center - half_h < other_position.y_center + other_half_h;
}

bool PhysicsComponent::is_collision(PhysicsComponent& other_physics, PositionComponent& other_position, PositionComponent& this_position) const
{
    const float half_w = _dimensions.width / 2.0f;
    const float half_h = _dimensions.height / 2.0f;

    const float other_half_w = other_physics._dimensions.width / 2.0f;
    const float other_half_h = other_physics._dimensions.height / 2.0f;

    return this_position.x_center + half_w > other_position.x_center - other_half_w && this_position.x_center - half_w < other_position.x_center + other_half_w &&
           this_position.y_center + half_h > other_position.y_center - other_half_h && this_position.y_center - half_h < other_position.y_center + other_half_h;
}
