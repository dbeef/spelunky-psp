//
// Created by xdbeef on 18.03.18.
//

#ifndef SPELUNKYDS_COLLISIONS_H
#define SPELUNKYDS_COLLISIONS_H


#include "../tiles/MapTile.hpp"

#define TILE_H 16
#define TILE_W 16

class Collisions {

public:

    //TODO Make equivalent functions that won't make x/y speed = 0 when collision == true (actually, just fix these)

    static bool checkUpperCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *ySpeed, int width, float bouncing_factor);

    static bool
    checkBottomCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *ySpeed, int width, int height, float bouncing_factor);

    static bool
    checkLeftCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *xSpeed, int width, int height, float bouncing_factor);

    static bool
    checkRightCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *xSpeed, int width, int height, float bouncing_factor);

    static bool isStandingOnLeftEdge(MapTile *neighboringTiles[9], int x, int width, int tileX);
    static bool isStandingOnRightEdge(MapTile *neighboringTiles[9], int x, int width, int tileX);

    static void getNeighboringTiles(MapTile *mapTiles[32][32], int xx, int yy, MapTile *out_neighboringTiles[9]);

    static void getTilesOnRightFromXY(int xx, int yy, MapTile *out_neighboringTiles[9]);
    static void getTilesOnLeftFromXY(int xx, int yy, MapTile *out_neighboringTiles[9]);

    static void bombNeighboringTiles(MapTile *mapTiles[32][32], int xx, int yy);

    static bool checkCollisionWithMainDude(int x, int y, int width, int height);

    static bool checkCollisionWithMainDudeWidthBoundary(int x, int y, int width, int height, int x_boundary);

    static bool checkCollisionWithMainDudeWhip(int x, int y, int width, int height);

    static bool checkCollisionBodies(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

    static bool checkCollisionBodiesLeftLowerCorner(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

};


#endif //SPELUNKYDS_COLLISIONS_H
