#pragma once

#include "MapTile.hpp"

namespace collisions
{
    /**
    * When it comes to checking collisions, we don't check every tile on the map if it collides
    * with i.e main dude. We check only the tiles that are neighboring to the main dude.
    * So we get main dude's position in tiles (range 0 - LEVEL_WIDTH/HEIGHT_TILES), make an array of the tiles that are on his left,
    * right, upper and lower sides and pass them to the collision detection function.
    * So this enum is for indicating which tile in the array is on which position, relative to the center tile,
    * in which (supposedly) is the thing on which we're checking collisions.
    *
    * 0 1 2
    * 3 4 5
    * 6 7 8
    *
    * In example, tile number 4 is the CENTER tile in this 9-element neighboring tiles array.
    */
    bool overlaps(MapTile* tile, float x_center, float y_center, float width, float height);
    MapTile* overlaps(MapTile* neighboring_tiles[9], float x_center, float y_center, float width, float height, bool collidable = true);
    MapTile* overlaps_strict(MapTile *neighboring_tiles[9], float x_center, float y_center, float width, float height, bool collidable = true);
}
