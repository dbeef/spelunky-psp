//
// Created by xdbeef on 15.09.18.
//

#ifndef SPELUNKYDS_LEVEL_LAYOUT_H
#define SPELUNKYDS_LEVEL_LAYOUT_H

#include "Level.hpp"

void generate_new_level_layout(Level *level);

void obtain_new_direction(int curr_x, Direction &direction);

void place_an_altar(Level *level);

void place_a_shop(Level *level);

#endif //SPELUNKYDS_LEVEL_LAYOUT_H
