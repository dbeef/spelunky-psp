//
// Created by xdbeef on 30.04.18.
//

#ifndef SPELUNKYDS_TILEORIENTATION_H
#define SPELUNKYDS_TILEORIENTATION_H

/**
 * When it comes to checking collisions, we don't check every tile on the map if it collides
 * with i.e main dude. We check only the tiles that are neighboring to the main dude.
 * So we get main dude's position in tiles (range 0-32), make an array of the tiles that are on his left,
 * right, upper and lower sides and pass them to the collision detection function.
 * So this enum is for indicating which tile in the array is on which position, relative to the center tile,
 * in which (supposedly) is the thing on which we're checking collisions.
 *
 * 1 2 3
 * 4 5 6
 * 7 8 9
 *
 * Tile number 5 is the CENTER tile in this 9 element neighboring tiles array.
 */
enum class TileOrientation {
    LEFT_MIDDLE, //0
    RIGHT_MIDDLE,//1
    UP_MIDDLE,   //2
    DOWN_MIDDLE, //3
    CENTER,      //4
    LEFT_UP,     //5
    RIGHT_UP,    //6
    LEFT_DOWN,   //7
    RIGHT_DOWN,  //8
};

#endif //SPELUNKYDS_TILEORIENTATION_H
