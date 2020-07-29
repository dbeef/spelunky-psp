//
// Created by xdbeef on 10.03.18.
//

#ifndef SPELUNKYDS_MAPTILE_H
#define SPELUNKYDS_MAPTILE_H

#include "MapTileType.hpp"

struct MapTile
{
    MapTile() : x(0), y(0), collidable(false), destroyable(false), exists(false), map_tile_type(MapTileType::NOTHING) {}

    int x; // in tiles
    int y; // in tiles
    bool collidable;
    bool destroyable;
    bool exists;
    MapTileType map_tile_type;

    // Sets tile's properties (i.e whether it is collidable or destroyable) to be of exact tile type.
    void match_tile(MapTileType type);
};


#endif //SPELUNKYDS_MAPTILE_H
