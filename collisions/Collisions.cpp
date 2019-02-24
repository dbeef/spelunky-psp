//fixme this shoudn't be a class

//
// Created by xdbeef on 18.03.18.
//
#include <cmath>
#include "Collisions.hpp"
#include "../entities/main_dude/MainDude.hpp"
#include "../GlobalsDeclarations.hpp"
#include "../tiles/TileOrientation.hpp"

//(TILE_W * a), (TILE_H * b) makes x,y point placed in the left-upper corner of the tile from passed *map_tiles[a][b].
//Sprite's x,y positions that are given to the following functions are expected to be upper-left corner.
//The x,y coordinates go from up to down on the y's, and from left to right on the x's - (0,0) is upper-left corner of
//the map.

//Collision from upper side of the tile, with rectangle given by x,y, and width.
bool Collisions::checkUpperCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *ySpeed, int width,
                                     float bouncing_factor) {

    bool w1;
    bool w2;

    for (int a = 0; a < 9; a++) {

        if (!neighboringTiles[a]->exists || !neighboringTiles[a]->collidable)
            continue;

        w1 = *yPos < (neighboringTiles[a]->y * TILE_H) + TILE_H && (*yPos > (neighboringTiles[a]->y * TILE_H));
        w2 = *xPos > (neighboringTiles[a]->x * TILE_W) - width &&
             (*xPos < (neighboringTiles[a]->x * TILE_W) + TILE_W);

        if (w1 && w2) {
            *ySpeed = (-1) * bouncing_factor * *ySpeed;
            *yPos = (neighboringTiles[a]->y * TILE_H) + TILE_H;
            return true;
        }

    }

    return false;
}

//Collision from bottom side of the tile, with rectangle given by x,y, width and height.
bool Collisions::checkBottomCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *ySpeed, int width,
                                      int height, float bouncing_factor) {

    bool w1;
    bool w2;

    for (int a = 0; a < 9; a++) {

        if (neighboringTiles[a] == nullptr || !neighboringTiles[a]->collidable)
            continue;

        bool collision;

        if (width >= 16 && height >= 16) {
            w1 = (*xPos > (neighboringTiles[a]->x * TILE_W) - (width * 0.75) &&
                  *xPos < (neighboringTiles[a]->x * TILE_W) + (width * 0.75));
            w2 = (*yPos <= neighboringTiles[a]->y * TILE_H) && *yPos + height >= (neighboringTiles[a]->y * TILE_H);
            collision = w1 && w2;
        } else {
            w1 = (*xPos > (neighboringTiles[a]->x * TILE_W) - (width) &&
                  *xPos < (neighboringTiles[a]->x * TILE_W) + (TILE_W));
            w2 = (*yPos <= neighboringTiles[a]->y * TILE_H) && *yPos + height >= (neighboringTiles[a]->y * TILE_H);
            collision = w1 && w2;
        }

        if (collision) {
            *ySpeed = (-1) * bouncing_factor * *ySpeed;

            if (fabs(*ySpeed) < 0.15f)
                *ySpeed = 0;

            *yPos = (neighboringTiles[a]->y * TILE_H) - height;
            return true;
        }
    }

    return false;
}

//Collision from left side of the tile, with rectangle given by x,y, width and height.
bool Collisions::checkLeftCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *xSpeed, int width,
                                    int height, float bouncing_factor) {

    bool w1;
    bool w2;

    for (int a = 0; a < 9; a++) {

        if (!neighboringTiles[a]->exists || !neighboringTiles[a]->collidable)
            continue;

        if (width == 16) {
            w2 = *xPos < (neighboringTiles[a]->x * TILE_W) - 0.75 * width &&
                 *xPos > (neighboringTiles[a]->x * TILE_W) - width;
        } else {
            w2 = (*xPos < (neighboringTiles[a]->x * TILE_W) - width + (TILE_W * 0.75) &&
                  (*xPos + width > (neighboringTiles[a]->x * TILE_W)));
        }

        w1 = (*yPos > (neighboringTiles[a]->y * TILE_H) - height &&
              (*yPos < (neighboringTiles[a]->y * TILE_H) + TILE_H));

        if (w1 && w2) {
            if (bouncing_factor > 0)
                *xSpeed = (-1) * bouncing_factor * *xSpeed;
            *xPos = (neighboringTiles[a]->x * TILE_W) - width;
            return true;
        }
    }

    return false;
}

//Collision from right side of the tile, with rectangle given by x,y, width and height.

//Collision from right side of the tile, with rectangle given by x,y, width and height.
bool Collisions::checkRightCollision(MapTile *neighboringTiles[9], int *xPos, int *yPos, double *xSpeed, int width,
                                     int height, float bouncing_factor) {
    bool w1;
    bool w2;
    for (int a = 0; a < 9; a++) {

        if (neighboringTiles[a] == nullptr || !neighboringTiles[a]->collidable)
            continue;

        if (width == 16) {
            w2 = (*xPos < (neighboringTiles[a]->x * TILE_W) + width &&
                  (*xPos > (neighboringTiles[a]->x * TILE_W) + 0.75 * width));
        } else {
            w2 = (*xPos < (neighboringTiles[a]->x * TILE_W) + TILE_W &&
                  (*xPos > (neighboringTiles[a]->x * TILE_W) + 0.75 * width));
        }

        w1 = (*yPos > (neighboringTiles[a]->y * TILE_H) - height &&
              (*yPos < (neighboringTiles[a]->y * TILE_H) + TILE_H));

        if (w1 && w2) {
            if (bouncing_factor > 0)
                *xSpeed = (-1) * bouncing_factor * *xSpeed;
            *xPos = (neighboringTiles[a]->x * TILE_W) + TILE_W;
            return true;
        }

    }

    return false;
}

bool Collisions::isStandingOnLeftEdge(MapTile *neighboringTiles[9], int x, int width, int tileX) {
    return (!neighboringTiles[TileOrientation::LEFT_DOWN] /*&& !neighboringTiles[7]->collidable && x <= (tileX * 16)*/);
}

bool Collisions::isStandingOnRightEdge(MapTile *neighboringTiles[9], int x, int width, int tileX) {
    return (!neighboringTiles[TileOrientation::RIGHT_DOWN] /*&& !neighboringTiles[8]->collidable && x >= (tileX * 16)*/);
}


//#include <cassert>
//
void Collisions::getNeighboringTiles(MapTile *mapTiles[32][32], int xx, int yy, MapTile *out_neighboringTiles[9]) {

//    if(xx < 0 || yy < 0 || xx >= 32 || yy >= 32){
    //        std::cout<< "WRONG" << xx << " " << yy;
//    }

//    assert(xx >= 0 && yy >= 0 && xx < 32 && yy < 32);


    MapTile *left_middle = nullptr,
            *right_middle = nullptr,
            *up_middle = nullptr,
            *down_middle = nullptr,
            *center = nullptr,
            *left_up = nullptr,
            *right_up = nullptr,
            *left_down = nullptr,
            *right_down = nullptr;

    if (xx > 0)
        left_middle = mapTiles[xx - 1][yy]->exists ? mapTiles[xx - 1][yy] : nullptr;
    if (xx < 31)
        right_middle = mapTiles[xx + 1][yy]->exists ? mapTiles[xx + 1][yy] : nullptr;
    if (yy > 0)
        up_middle = mapTiles[xx][yy - 1]->exists ? mapTiles[xx][yy - 1] : nullptr;
    if (yy < 31)
        down_middle = mapTiles[xx][yy + 1]->exists ? mapTiles[xx][yy + 1] : nullptr;

    center = mapTiles[xx][yy]->exists ? mapTiles[xx][yy] : nullptr;

    if (xx > 0 && yy > 0)
        left_up = mapTiles[xx - 1][yy - 1]->exists ? mapTiles[xx - 1][yy - 1] : nullptr;
    if (xx < 31 && yy > 0)
        right_up = mapTiles[xx + 1][yy - 1]->exists ? mapTiles[xx + 1][yy - 1] : nullptr;
    if (xx > 0 && yy < 31)
        left_down = mapTiles[xx - 1][yy + 1]->exists ? mapTiles[xx - 1][yy + 1] : nullptr;
    if (xx < 31 && yy < 31)
        right_down = mapTiles[xx + 1][yy + 1]->exists ? mapTiles[xx + 1][yy + 1] : nullptr;

    out_neighboringTiles[TileOrientation::LEFT_MIDDLE] = left_middle;
    out_neighboringTiles[TileOrientation::RIGHT_MIDDLE] = right_middle;
    out_neighboringTiles[TileOrientation::UP_MIDDLE] = up_middle;
    out_neighboringTiles[TileOrientation::DOWN_MIDDLE] = down_middle;
    out_neighboringTiles[TileOrientation::CENTER] = center;
    out_neighboringTiles[TileOrientation::LEFT_UP] = left_up;
    out_neighboringTiles[TileOrientation::RIGHT_UP] = right_up;
    out_neighboringTiles[TileOrientation::LEFT_DOWN] = left_down;
    out_neighboringTiles[TileOrientation::RIGHT_DOWN] = right_down;

}

void Collisions::bombNeighboringTiles(MapTile *mapTiles[32][32], int xx, int yy) {
    mapTiles[xx - 1][yy]->exists = !mapTiles[xx - 1][yy]->destroyable;
    mapTiles[xx + 1][yy]->exists = !mapTiles[xx + 1][yy]->destroyable;
    mapTiles[xx][yy - 1]->exists = !mapTiles[xx][yy - 1]->destroyable;
    mapTiles[xx][yy + 1]->exists = !mapTiles[xx][yy + 1]->destroyable;
    mapTiles[xx][yy]->exists = !mapTiles[xx][yy]->destroyable;
    mapTiles[xx - 1][yy - 1]->exists = !mapTiles[xx - 1][yy - 1]->destroyable;
    mapTiles[xx + 1][yy - 1]->exists = !mapTiles[xx + 1][yy - 1]->destroyable;
    mapTiles[xx - 1][yy + 1]->exists = !mapTiles[xx - 1][yy + 1]->destroyable;
    mapTiles[xx + 1][yy + 1]->exists = !mapTiles[xx + 1][yy + 1]->destroyable;
    mapTiles[xx][yy + 2]->exists = !mapTiles[xx][yy + 2]->destroyable;
}

bool Collisions::checkCollisionWithMainDude(int x, int y, int width, int height) {
    return checkCollisionBodies(x, y, width, height, global::main_dude->_x, global::main_dude->_y, 16, 16);
}

bool Collisions::checkCollisionWithMainDudeWidthBoundary(int x, int y, int width, int height, int x_boundary) {
    return global::main_dude->_x + 16 > x && global::main_dude->_x <= x + width &&
           y + height > global::main_dude->_y && y <= global::main_dude->_y + MainDude::main_dude_physical_height;
}

bool Collisions::checkCollisionWithMainDudeWhip(int x, int y, int width, int height) {
    if (global::main_dude->sprite_state == Orientation::LEFT) {
        return x + width >= global::main_dude->_x - WHIP_WIDTH &&
               x + width < global::main_dude->_x + MainDude::main_dude_physical_width &&
               y + height > global::main_dude->_y && y < global::main_dude->_y + MainDude::main_dude_physical_height;
    } else if (global::main_dude->sprite_state == Orientation::RIGHT) {
        return x + width >= global::main_dude->_x &&
               x + width < global::main_dude->_x + MainDude::main_dude_physical_width + WHIP_WIDTH * 2 &&
               y + height > global::main_dude->_y && y < global::main_dude->_y + MainDude::main_dude_physical_height;
    } else return false;

}

//x1,y1 is a left >upper< corner of the body, same for the x2,y2
bool Collisions::checkCollisionBodies(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    return x1 + w1 > x2 && x1 < x2 + w2 && y1 + h1 > y2 && y1 < y2 + h2;
}

//x1,y1 is a left >lower< corner of the body, same for the x2,y2
bool Collisions::checkCollisionBodiesLeftLowerCorner(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    return x1 + w1 > x2 && x1 < x2 + w2 && y1 > y2 - h2 && y1 - h1 < y2;
}

//returns 9 tiles from given tile x/y
void Collisions::getTilesOnRightFromXY(int xx, int yy, MapTile **out_neighboringTiles) {
    for (int a = 0; a < 9; a++) {
        if (a + xx <= 31)
            out_neighboringTiles[a] = global::current_level->map_tiles[a + xx][yy];
        else
            out_neighboringTiles[a] = nullptr;
    }
}

//returns 9 tiles from given tile x/y
void Collisions::getTilesOnLeftFromXY(int xx, int yy, MapTile **out_neighboringTiles) {
    for (int a = 0; a < 9; a++) {
        if (xx - a >= 0)
            out_neighboringTiles[a] = global::current_level->map_tiles[xx - a][yy];
        else
            out_neighboringTiles[a] = nullptr;
    }
}

