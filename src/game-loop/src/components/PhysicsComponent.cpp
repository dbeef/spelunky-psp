#include "components/PhysicsComponent.hpp"
#include "MainDude.hpp"
#include "LevelGenerator.hpp"
#include "Collisions.hpp"

#include <cmath>
#include <logger/log.h>
#include <components/PhysicsComponent.hpp>

namespace
{
    constexpr double default_max_x_speed = 3.0f;
    constexpr double default_max_y_speed = 3.5f;
    constexpr double default_friction = 0.055f;
    constexpr float default_gravity = 0.19f;
    constexpr float default_bouncing_factor_x = 0.15f;
    constexpr float default_bouncing_factor_y = 0.35f;
    constexpr uint16_t default_pos_update_delta_ms = 15;
}

void PhysicsComponent::update(MainDude &main_dude, uint32_t delta_time_ms)
{
    _pos_update_delta_ms += delta_time_ms;

    while (_pos_update_delta_ms >= default_pos_update_delta_ms)
    {
        _pos_update_delta_ms -= default_pos_update_delta_ms;

        MapTile *neighbours[9] = {};
        collisions::get_neighbouring_tiles(LevelGenerator::instance().getLevel(), main_dude._x, main_dude._y, neighbours);

        auto bottom_tile = neighbours[static_cast<int>(collisions::NeighbouringTiles::CENTER)];

        if (bottom_tile != nullptr)
        {
            if (bottom_tile->mapTileType == MapTileType::EXIT)
            {
                log_info("Overlaping exit tile!");
            }
        }

        auto collision_tile = collisions::check_bottom_collision(neighbours, main_dude._x, main_dude._y, _dimensions.width, _dimensions.height);
        if (collision_tile)
        {
            log_info("Collision with tile: %i", collision_tile->mapTileType);
        }

        // TODO: left/right/bottom/top collisions
    }
}

PhysicsComponent::PhysicsComponent(float width, float height) : _dimensions{width, height}
{

}
