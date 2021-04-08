#pragma once

#include "other/PhysicsComponentType.hpp"
#include "PositionComponent.hpp"
#include "ZoneComponent.hpp"

#include <cstdint>
#include <functional>
#include <utility>

class PhysicsComponent
{
public:
    PhysicsComponent(float width, float height);
    PhysicsComponent() = default;

    bool is_collision(PhysicsComponent& other_physics, PositionComponent& other_position, PositionComponent& this_position) const;
    bool is_collision(ZoneComponent& other_zone, PositionComponent& other_position, PositionComponent& this_position) const;

    void update(uint32_t delta_time_ms,PositionComponent& position);

    void set_dimensions(float width, float height) { _dimensions.width = width; _dimensions.height = height; }
    float get_width() const { return _dimensions.width; }
    float get_height() const { return _dimensions.height; }

    void set_velocity(float x, float y) { _velocity.x = x; _velocity.y = y; }
    void set_x_velocity(float x) { _velocity.x = x;}
    void set_y_velocity(float y) { _velocity.y = y; }
    void add_velocity(float x, float y) { _velocity.x += x; _velocity.y += y; }

    float get_x_velocity() const { return _velocity.x; }
    float get_y_velocity() const { return _velocity.y; }

    void set_max_x_velocity(float x) { _velocity.max_x = x;}
    void set_max_y_velocity(float y) { _velocity.max_y = y;}

    float get_max_x_velocity() const { return _velocity.max_x; }
    float get_max_y_velocity() const { return _velocity.max_y; }

    void set_gravity_modifier(float modifier) { _properties.gravity_modifier = modifier; }

    void disable_gravity() { _gravity = false; };
    void enable_gravity() { _gravity = true; };

    bool is_bottom_collision() const { return _collisions.bottom; }
    bool is_upper_collision() const { return _collisions.upper; }
    bool is_right_collision() const { return _collisions.right; }
    bool is_left_collision() const { return _collisions.left; }

    void set_bounciness(float bounciness) { _properties.bounciness = bounciness; }
    void set_friction(float friction) { _properties.friction = friction; }

    static float get_default_friction() { return 0.005f; }
    static float get_default_max_x_velocity() { return 0.35f; }
    static float get_default_max_y_velocity() { return 0.39f; }

private:

    struct
    {
        float width = 0;
        float height = 0;
    } _dimensions;

    struct
    {
        float x = 0;
        float y = 0;
        float max_x = get_default_max_x_velocity();
        float max_y = get_default_max_y_velocity();
    } _velocity;

    struct
    {
        bool bottom = false;
        bool upper = false;
        bool left = false;
        bool right = false;
    } _collisions;

    struct
    {
        float bounciness = 0.0f;
        float friction = PhysicsComponent::get_default_friction();
        float gravity_modifier = 1.0f;
    } _properties;

    bool _gravity = true;
    int32_t _pos_update_delta_ms = 0;
};
