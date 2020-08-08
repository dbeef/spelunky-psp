//
// Created by xdbeef on 00.00.00.
//

#ifndef SPELUNKYDS_SHOPS_H
#define SPELUNKYDS_SHOPS_H

//shop(s) for different level themes (cave/jungle/etc) and for different orientations (left/right)
static const int shops[2][10][10] =
        {
                //cave left
                {
                        {2, 2, 2, 2, 2, 2, 2, 2, 5, 2},
                        {2, 2, 2, 2, 2, 6, 2, 2, 2, 2},
                        {2, 2, 2, 5, 2, 2, 2, 2, 2, 6},
                        {2, 5, 2, 2, 2, 4, 2, 6, 2, 2},
                        {2, 2, 4, 4, 4, 0, 4, 2, 2, 2},
                        {3, 2, 0, 0, 0, 0, 0, 2, 2, 2},
                        {0, 4, 0, 0, 0, 0, 0, 2, 2, 2},
                        {0, 29, 0, 0, 0, 0, 0, 4, 2,  5},
                        {0, 0, 0, 0, 0, 0, 0, 0, 4, 4},
                        {24, 24, 24, 24, 24, 24, 24, 24, 24, 24},
                },
                //cave right
                {
                        {2, 5, 2, 2, 2, 2, 2, 2, 2, 2},
                        {2, 2, 2, 2, 6, 2, 2, 2, 2, 2},
                        {6, 2, 2, 2, 2, 2, 5, 2, 2, 2},
                        {2, 2, 6, 2, 4, 2, 2, 2, 5, 2},
                        {2, 2, 2, 4, 0, 4, 4, 4, 2, 2},
                        {2, 2, 2, 0, 0, 0, 0, 0, 2, 3},
                        {2, 2, 2, 0, 0, 0, 0, 0, 4, 0},
                        {5, 2,  4, 0, 0, 0, 0, 0, 29, 0},
                        {4, 4, 0, 0, 0, 0, 0, 0, 0, 0},
                        {24, 24, 24, 24, 24, 24, 24, 24, 24, 24},
                }
        };

//shop(s) for different level themes (cave/jungle/etc) and for different orientations (left/right)
static const int shops_mugshots[2][10][10] =
        {
                //cave left
                {
                        {2, 2, 2, 2, 2, 2, 2, 2, 5, 2},
                        {2, 2, 2, 2, 2, 6, 2, 2, 2, 2},
                        {2, 5, 2, 2, 2, 4, 2, 6, 2, 2},
                        {2, 2, 4, 4, 4, 0, 4, 2, 2, 2},
                        {3, 2, 0, 0, 0, 0, 0, 2, 2, 2},
                        {2, 2, 2, 5, 2, 4, 4, 4, 2, 6},
                        {0, 4, 37, 38, 0, 0, 0,  2,  2, 2},
                        {0, 29, 39, 40, 0, 0, 0,  4,  2,  5},
                        {0, 0, 0, 0, 0, 0, 0, 0, 4, 4},
                        {24, 24, 24, 24, 24, 24, 24, 24, 24, 24},
                },
                //cave right
                {
                        {2, 5, 2, 2, 2, 2, 2, 2, 2, 2},
                        {2, 2, 2, 2, 6, 2, 2, 2, 2, 2},
                        {6, 2, 2, 2, 2, 2, 5, 2, 2, 2},
                        {2, 2, 6, 2, 4, 2, 2, 2, 5, 2},
                        {2, 2, 2, 4, 0, 4, 4, 4, 2, 2},
                        {2, 2, 2, 0, 0, 0, 0, 0, 2, 3},
                        {2, 2, 2,  0,  0, 0, 37, 38, 4, 0},
                        {5, 2,  4,  0,  0, 0, 39, 40, 29, 0},
                        {4, 4, 0, 0, 0, 0, 0, 0, 0, 0},
                        {24, 24, 24, 24, 24, 24, 24, 24, 24, 24},
                }
        };

//only shopkeeper[8] + lamp[7]
//12 is random item
static const int shops_npcs[3][10][10] =
        {
                {
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 7, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 12, 12, 12, 12, 8, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                },
                {
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 7, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 8, 12, 12, 12, 12, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                },
                {
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0,  0,  0,  0,  0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                },

        };

//items to be bought
static const int shops_loot[3][10][10] =
        {

                {
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                },
                {
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                },
                {
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                },
        };


#endif //SPELUNKYDS_SHOPS_H
