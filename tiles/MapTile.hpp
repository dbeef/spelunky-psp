//
// Created by xdbeef on 10.03.18.
//

#ifndef SPELUNKYDS_MAPTILE_H
#define SPELUNKYDS_MAPTILE_H

#include "MapTileType.h"

class MapTile {

public:

    MapTile() : x(0), y(0), collidable(false), destroyable(false), exists(false), mapTileType(NOTHING)
    {

    }

    int x;
    int y;
    bool collidable;
    bool destroyable;
    bool exists;
    MapTileType mapTileType;

    void match_tile(MapTileType type);
};


#endif //SPELUNKYDS_MAPTILE_H
