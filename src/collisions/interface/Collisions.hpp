#pragma once

#include "MapTile.hpp"
#include "Level.hpp"

namespace collisions
{
    enum class NeighbouringTiles
    {
        LEFT_MIDDLE,
        RIGHT_MIDDLE,
        UP_MIDDLE,
        DOWN_MIDDLE,
        CENTER,
        LEFT_UP,
        RIGHT_UP,
        LEFT_DOWN,
        RIGHT_DOWN,
    };

    void get_neighbouring_tiles(Level&, float x, float y, MapTile *out_neighboring_tiles[9]);
}
