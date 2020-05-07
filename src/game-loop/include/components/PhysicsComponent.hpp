#pragma once

#include <cstdint>
#include "MapTile.hpp"

class MainDude;

class PhysicsComponent
{
public:
    PhysicsComponent(float width, float height);
    void update(MainDude&, uint32_t delta_time_ms);
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
    } _velocity;

    struct
    {
        bool bottom = false;
        bool upper = false;
        bool left = false;
        bool right = false;
    } _collisions;

    MapTile *_neighboring_tiles[9]{};
    float _pos_update_delta_ms = 0;
};
