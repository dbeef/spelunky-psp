#pragma once

#include <cstdint>
#include "MapTile.hpp"

class MainDude;

class PhysicsComponent
{
public:

    // TODO: void set_max_speed(float x, float y);

    PhysicsComponent(float width, float height);
    void update(MainDude&, uint32_t delta_time_ms);

    void set_position(float x, float y) { _position.x = x; _position.y = y;}
    float get_x_position() const { return _position.x; }
    float get_y_position() const { return _position.y; }

    float get_width() const { return _dimensions.width; }
    float get_height() const { return _dimensions.height; }

    void add_velocity(float x, float y) { _velocity.x += x; _velocity.y += y; }
    float get_x_velocity() const { return _velocity.x; }
    float get_y_velocity() const { return _velocity.y; }

    bool is_bottom_collision() const { return _collisions.bottom; }
    bool is_upper_collision() const { return _collisions.upper; }
    bool is_right_collision() const { return _collisions.right; }
    bool is_left_collision() const { return _collisions.left; }

private:

    struct
    {
        float x = 0;
        float y = 0;
    } _position; // Position of the CENTER of the body

    struct
    {
        float width = 0;
        float height = 0;
    } _dimensions;

    struct
    {
        float x = 0;
        float y = 0;
    } _velocity;

    struct
    {
        bool bottom = false;
        bool upper = false;
        bool left = false;
        bool right = false;
    } _collisions;

    float _pos_update_delta_ms = 0;
};
