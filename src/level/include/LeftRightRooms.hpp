#pragma once

static const int left_right_rooms[6][10][10] =
        {
                {
                        { 3,  4,  4,  4,  4,  4,  4,  4,  4,  3},
                        { 9,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        {10,  8,  8,  8,  8,  8,  8,  8,  0,  0},
                        { 9,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 9,  0,  0,  0,  0,  0,  7,  7,  0,  0},
                        { 9,  0,  0,  0,  0,  0,  4,  4,  8,  0},
                        { 9,  0,  0,  0,  7,  0,  0,  0,  0,  0},
                        { 9,  7,  7,  0,  6,  0,  0,  0,  0,  0},
                        { 7,  5,  5,  7,  2,  7,  0,  0,  0,  0},
                        { 1,  2,  2,  2,  2,  2,  7,  7,  7,  7},
                },
                {
                        { 3,  4,  4,  4,  4,  4,  4,  4,  4,  3},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  9,  0,  0,  0,  0,  0,  0,  0},
                        { 7,  7, 10,  7,  7,  0,  0,  0,  0,  7},
                        { 2,  2,  9,  2,  2,  0,  0,  0,  0,  3},
                        { 1,  2,  9,  2,  2,  0,  0,  0,  0,  0},
                        { 3,  2,  9,  2,  2,  0,  0,  0,  0,  0},
                        { 0,  4,  9,  4,  4,  0,  7,  7,  0,  0},
                        { 0,  0,  9,  0,  0,  0,  2,  5,  0,  0},
                        { 7,  7,  7,  7,  7,  7,  2,  2,  7,  7},
                },
                {
                        { 3,  4,  4,  4,  4,  4,  4,  4,  4,  1},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  4},
                        { 0,  0,  0,  7,  0,  0,  0,  0,  0,  0},
                        { 7,  7,  7,  2,  8,  8,  0,  0,  0,  7},
                        { 3,  6,  2,  4,  0,  0,  0,  0,  7,  5},
                        { 0,  4,  4,  0,  0,  0,  0,  8,  4,  3},
                        { 0,  0,  0,  0,  0,  7,  0,  0,  0,  0},
                        { 0,  0,  0,  0, 11,  5,  0,  0,  0,  0},
                        { 0,  7,  7,  7,  2,  2,  7,  0,  0,  0},
                        { 7,  2,  2,  6,  2,  2,  2,  7,  7,  7},
                },
                {
                        { 2,  6,  4,  0,  0,  0,  0,  4,  2,  3},
                        { 3,  2,  0,  0,  0,  0,  0,  0,  4,  0},
                        { 0,  4,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  9,  0,  0,  0,  0,  9,  0,  0},
                        { 0,  0, 10,  8,  8,  8,  8, 10,  0,  0},
                        { 0,  0,  9,  0,  0,  0,  0,  9,  0,  0},
                        { 0,  0, 10,  7,  7,  7,  7, 10,  0,  3},
                        { 0,  0,  9,  2,  2,  2,  4,  9,  0,  8},
                        { 7,  0,  9,  2,  6,  2, 12,  9,  0,  0},
                        { 2,  7,  7,  2,  2,  2,  2,  7,  7,  7},
                },
                {
                        { 0,  0,  0,  0,  0,  0,  4,  5,  2,  5},
                        { 0,  0,  8,  8,  0,  0,  0,  4,  4,  4},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  9},
                        { 0,  0,  0,  0,  0,  0,  0,  7,  7, 10},
                        { 0,  8,  8,  8,  8,  0,  0,  2,  2,  9},
                        { 0,  0,  0,  0,  0,  0,  0,  2,  6,  9},
                        { 0,  0,  0,  0,  0,  0,  0,  2,  2,  9},
                        { 0,  0,  0,  0,  8,  8,  8,  4,  4,  9},
                        { 7,  7,  7,  0,  0,  0,  0,  0,  0,  9},
                        { 1,  2,  2,  7,  7,  7,  7,  7,  7,  7 },
                },
                {
                        { 0,  0,  0,  0,  0,  0,  4,  2,  2,  1},
                        { 0,  8,  8,  8,  8,  0,  0,  4,  4,  3},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  8,  8,  8,  8,  8, 12,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 7,  7,  7,  0,  0,  0,  0,  7,  0,  0},
                        { 3,  4,  2,  8,  0,  0,  7,  5,  0,  0},
                        { 0,  0,  4,  0,  0,  8,  4,  4,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0, 11},
                        { 7,  7,  7,  7,  7,  7,  7,  7,  7,  2},
                }
        };


static const int left_right_rooms_loot[6][10][10] =
        {
                {
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  1,  1,  1,  1,  1,  1,  1,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  1,  1,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  1,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  1,  1,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  1,  0,  1,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  1,  1,  1,  1},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                },{
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 1,  1,  0,  1,  1,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  1,  1,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 1,  1,  0,  1,  1,  1,  0,  0,  1,  1},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                },{
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 1,  1,  1,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  1,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  1,  1,  1,  0,  0,  1,  0,  0,  0},
                        { 1,  0,  0,  0,  0,  0,  0,  1,  1,  1},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                },{
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  1,  1,  1,  1,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  1,  1,  1,  1,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 1,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  1,  0,  0,  0,  0,  0,  0,  1,  1},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                },{
                        { 0,  0,  1,  1,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  1,  1,  0},
                        { 0,  1,  1,  1,  1,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  1,  1,  1,  0,  0,  0},
                        { 1,  1,  1,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  1,  1,  1,  1,  1,  1,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                },{
                        { 0,  1,  1,  1,  1,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  1,  1,  1,  1,  1,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 1,  1,  1,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  1,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 1,  1,  1,  1,  1,  1,  1,  1,  1,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                },
        };

static const int left_right_rooms_npc[6][10][10] =
        {
                {
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  1,  1,  1,  1,  1,  1,  1,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  1,  1,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  1,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  1,  1,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  1,  0,  1,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  1,  1,  1,  1},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                },{
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 1,  1,  0,  1,  1,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  1,  1,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 1,  1,  0,  1,  1,  1,  0,  0,  1,  1},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                },{
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 1,  1,  1,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  1,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  1,  1,  1,  2,  0,  1,  0,  0,  0},
                        { 1,  0,  0,  0,  0,  0,  0,  1,  1,  1},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                },{
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  1,  1,  1,  1,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  1,  1,  1,  1,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 1,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  1,  0,  0,  0,  0,  0,  0,  1,  1},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                },{
                        { 0,  0,  1,  1,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  1,  1,  0},
                        { 0,  1,  1,  1,  1,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  1,  1,  1,  0,  0,  0},
                        { 1,  1,  1,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  1,  1,  1,  1,  1,  1,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                },{
                        { 0,  1,  1,  1,  1,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  1,  1,  1,  1,  1,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 1,  1,  1,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  1,  0,  0,  0,  0},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                        { 1,  1,  1,  1,  1,  1,  1,  1,  1,  2},
                        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                },
        };
