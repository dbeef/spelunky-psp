//
// Created by xdbeef on 15.09.18.
//

#include <cstdlib>
#include "Level.hpp"
#include "LevelGenerator.hpp"
#include "../rooms/RoomType.hpp"

/**
 * How the Spelunky level generator works:
 *
 * http://tinysubversions.com/spelunkyGen/
 * https://www.youtube.com/watch?v=ouh7EZ5Qh9g
 *
 * My implementation is based on the resources linked above.
 * index (0, 2) on the level.layout array is the upper-left room on the 3x3 room map.
 */
void generate_new_level_layout(Level *level) {

    //clean current layout
    level->clean_map_layout();
    for (int x = 0; x < ROOMS_X; x++)
        for (int y = 0; y < ROOMS_Y; y++)
            //not visited rooms are of CLOSED type by default
            level->layout[x][y] = R_CLOSED;


    //generate new seed for the random number generator
//    srand(timerElapsed(1));
    srand(NULL);

    //set starting position to the random room in the most upper row
    int curr_x = rand() % 3;
    int curr_y = ROOMS_Y - 1;
    //direction represents where the generator will go in the next loop iteration
    Direction direction;
    obtain_new_direction(curr_x, direction);

    bool exit_placed = false;

    //set the starting room as an entrance room
    level->layout[curr_x][curr_y] = R_ENTRANCE;

    //while we're on the very bottom floor or higher, do
    while (curr_y >= 0) {

        if (direction == LEFT || direction == RIGHT) {

            if ((direction == LEFT && curr_x == 0) || (direction == RIGHT && curr_x == 2)) {
                //our direction is to go left, but we're already on the left side of the map, so go down
                direction = DOWN;
            } else {

                if (direction == LEFT)
                    //our direction is to go left, and we're not on the left side of the map yet
                    curr_x--;
                else
                    //same, if right side
                    curr_x++;

                if (curr_y == 0 && !exit_placed && rand() % 2 == 0) {
                    //we're on the most bottom floor, we didn't plant an exit yet and we've guessed that's the place
                    exit_placed = true;
                    level->layout[curr_x][curr_y] = R_EXIT;
                } else
                    level->layout[curr_x][curr_y] = R_LEFT_RIGHT;

                if (rand() % 3 == 2)
                    //random chance that we change our direction to go down in the next iteration
                    direction = DOWN;
            }

        } else if (direction == DOWN) {

            if (curr_y > 0) {

                level->layout[curr_x][curr_y] = R_LEFT_RIGHT_DOWN;
                curr_y--;
                level->layout[curr_x][curr_y] = R_LEFT_RIGHT_UP;

                if (curr_y == 0 && !exit_placed && rand() % 2 == 0) {
                    //if we're on the very bottom floor, no exit planted yet and a guess tells us so, place an exit
                    exit_placed = true;
                    level->layout[curr_x][curr_y] = R_EXIT;
                }

                obtain_new_direction(curr_x, direction);
            } else {

                if (!exit_placed)
                    //we're on the very bottom floor, didn't plant an exit yet and we're
                    //done with iterating through map, so plant an exit
                    level->layout[curr_x][curr_y] = R_EXIT;

                break;
            }

        }
    }

    //TODO more post-generation effects, i.e if there's a column of '0' type rooms, then make a snake well
    place_an_altar(level);
    place_a_shop(level);
}

/**
 * Used in the process of placing rooms.
 * !\relates generate_new_rooms
 */
void obtain_new_direction(int curr_x, Direction &direction) {
    if (curr_x == 0)
        //we're on the left side of the map, so go right
        direction = RIGHT;
    else if (curr_x == 2)
        //we're on the right side of the map, so go left
        direction = LEFT;
    else
        //we're in the middle, so make a guess where should we gow now
        direction = static_cast<Direction>(rand() % 2); //left or right
}

void place_an_altar(Level *level) {
    for (int a = 0; a < ROOMS_X; a++) {
        for (int b = 0; b < ROOMS_Y; b++) {
            if (level->layout[a][b] == R_CLOSED) {
                level->layout[a][b] = R_ALTAR;
                return;
            }
        }
    }
}


/**
 * Finds a closed room that is not blocked from left or right side by other closed room,
 * and plants a shop there that is oriented to the not-blocked side.
 * !\relates generate_new_rooms
 */
void place_a_shop(Level *level) {
    for (int a = 0; a < ROOMS_X; a++) {
        for (int b = 0; b < ROOMS_Y; b++) {
            if (level->layout[a][b] == R_CLOSED) {
                if (a == 0) {
                    if (level->layout[a + 1][b] != R_CLOSED) {
//                        if (global::game_state->robbed_killed_shopkeeper)
                        if (false)
                            level->layout[a][b] = R_SHOP_RIGHT_MUGSHOT;
                        else
                            level->layout[a][b] = R_SHOP_RIGHT;
                        return;
                    }
                } else if (a == 2) {
                    if (level->layout[a - 1][b] != R_CLOSED) {
//                        if (global::game_state->robbed_killed_shopkeeper)
                        if (false)
                            level->layout[a][b] = R_SHOP_LEFT_MUGSHOT;
                        else
                            level->layout[a][b] = R_SHOP_LEFT;
                        return;
                    }
                } else if (a == 1) {
                    if (level->layout[a - 1][b] != R_CLOSED &&
                        level->layout[a + 1][b] != R_CLOSED) {

                        if (rand() % 2 == 0)
                            level->layout[a][b] = R_SHOP_LEFT;
                        else
                            level->layout[a][b] = R_SHOP_RIGHT;

                        return;
                    }
                }
            }
        }
    }
}


