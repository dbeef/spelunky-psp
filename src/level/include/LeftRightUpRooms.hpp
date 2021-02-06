#pragma once


static const int left_right_up_rooms[6][10][10] =
        {
                {
                        { 3,  0,  0,  0,  0,  0,  0,  0,  0,  6},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  3},
                        { 0,  0,  8,  8,  8,  8,  8,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  9,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0, 10,  7,  7,  7},
                        { 7,  7, 12,  0,  0,  0,  9,  4,  4,  3},
                        { 3,  4,  4,  0,  0,  0,  9,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  7,  0,  0,  0},
                        { 0,  7,  7,  7,  0,  7,  5,  7,  0,  0},
                        { 7,  2,  2,  5,  7,  2,  2,  2,  7,  7}
                },
                {
                        { 3,  0,  0,  0,  0,  0,  0,  0,  0,  6},
                        { 0,  7,  7,  0,  0,  0,  0,  0,  0,  4},
                        { 0,  2,  6,  0,  0,  9,  0,  0,  0,  0},
                        { 0,  4,  4,  8,  8, 10,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  9,  0,  0,  0,  7},
                        { 0,  0,  8,  8,  8,  9,  0,  0,  0,  3},
                        { 0,  0,  0,  0,  0,  9,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  9,  7,  8,  8,  0},
                        { 0,  0,  0,  0,  0,  9,  5,  0,  0,  0},
                        { 7,  7,  7,  7,  7,  7,  2,  7,  7,  7}
                },
                {
                        { 3,  0,  0,  0,  0,  0,  0,  0,  0,  6},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0, 11},
                        { 0,  0,  0,  8,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  7,  7,  8,  0,  0,  0,  0,  7,  7},
                        { 0,  4,  4,  0,  0,  0,  0,  0,  4,  3},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  8,  8,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  7,  0,  0},
                        { 7,  7,  7,  7,  7,  7,  7,  2,  7,  7}
                },
                {
                        { 0,  4,  4,  2,  4,  0,  0,  0,  0,  2},
                        { 0,  0,  0,  4,  0,  0,  0,  0,  8,  2},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0, 11},
                        { 0,  9,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0, 10,  8,  8,  8,  8,  7,  0,  0,  0},
                        { 0,  9,  0,  0,  0,  0,  2,  0,  0,  0},
                        { 0,  9,  0,  7,  7,  0,  4,  7,  0,  0},
                        { 0,  9,  7,  4,  2,  0,  0,  6,  7,  0},
                        { 7,  8,  4,  4,  2,  7,  7,  7,  2,  0},
                        { 1,  0,  0,  0,  2,  2,  5,  2,  2,  7},
                },
                {
                        { 6,  4,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 1,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 3,  8,  0,  0,  0,  0, 11,  8,  8,  8},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  9,  0},
                        { 0,  7,  8,  8,  8,  7,  7,  7, 10,  0},
                        { 0,  2,  0,  0,  0,  2,  2,  4,  9,  0},
                        { 7,  4,  0,  0,  0,  4,  4,  0,  9,  0},
                        { 1,  0,  0,  0,  0,  0,  0,  0,  0,  7},
                        { 2,  7,  7,  7,  7,  7,  7,  7,  7,  5},
                },
                {
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        {12,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 7,  7,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 2,  6,  9,  0,  0,  0,  0,  0,  0,  0},
                        { 3,  4, 10,  8,  8,  8,  8,  8,  7,  0},
                        { 0,  0,  9,  0,  0,  0,  0,  0,  4,  0},
                        { 7,  0,  9,  8,  8,  8,  8,  0, 11,  7},
                        { 6,  0,  9,  0,  0,  0,  0,  0,  0,  6},
                        { 1,  7,  7,  7,  7,  7,  7,  7,  7,  1},
                }
        };

static const int left_right_up_rooms_loot[6][10][10] =
        {
                {
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  1,  1,  1,  1,  1,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  1,  1,  1},
                        { 1,  1,  1,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  1,  1,  1,  0,  1,  0,  1,  0,  0},
                        { 1,  0,  0,  0,  1,  0,  0,  0,  1,  1},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                },{
                        { 0,  1,  1,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  1,  1,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  1,  1,  1,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 1,  1,  1,  1,  1,  0,  0,  1,  1,  1},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                },{
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  1,  1,  1,  0,  0,  0,  0,  1,  1},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 1,  1,  1,  1,  1,  1,  1,  0,  1,  1},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                },{
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  1,  1,  1,  1,  1,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  1,  0,  0},
                        { 0,  0,  1,  0,  0,  0,  0,  0,  1,  0},
                        { 1,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  1},
                        { 0,  1,  1,  1,  0,  0,  0,  0,  0,  0},
                },{
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  1,  1,  1,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  1,  1,  1,  1,  1,  1,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 1,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  1,  1,  1,  1,  1,  1,  1,  1,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                },{
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  1,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  1,  1,  1,  1,  1,  1,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  1,  1,  1,  1,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  1,  1,  1,  1,  1,  1,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                },
        };

static const int left_right_up_rooms_npc[6][10][10] =
        {
                {
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  1,  1,  1,  1,  1,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  1,  1,  1},
                        { 1,  1,  1,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  1,  1,  1,  0,  1,  0,  1,  0,  0},
                        { 1,  0,  0,  0,  1,  0,  0,  0,  1,  1},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                },{
                        { 0,  1,  1,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  1,  1,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  1,  1,  1,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 1,  1,  1,  1,  1,  0,  0,  1,  1,  1},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                },{
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  2},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  1,  1,  1,  0,  0,  0,  0,  1,  1},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 1,  1,  1,  1,  1,  1,  1,  0,  1,  1},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                },{
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  2},
                        { 0,  0,  1,  1,  1,  1,  1,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  1,  0,  0},
                        { 0,  0,  1,  0,  0,  0,  0,  0,  1,  0},
                        { 1,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  1},
                        { 0,  1,  1,  1,  0,  0,  0,  0,  0,  0},
                },{
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  1,  1,  1,  0},
                        { 0,  0,  0,  0,  0,  0,  2,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  1,  1,  1,  1,  1,  1,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 1,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  1,  1,  1,  1,  1,  1,  1,  1,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                },{
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  1,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  1,  1,  1,  1,  1,  1,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  1,  1,  1,  1,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  2,  0},
                        { 0,  0,  0,  1,  1,  1,  1,  1,  1,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                },
        };
