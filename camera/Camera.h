//
// Created by dbeef on 2/23/19.
//

#ifndef SPELUNKYPSP_CAMERA_H
#define SPELUNKYPSP_CAMERA_H

#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include "../utils/Consts.h"

class Camera {

public:

    Camera()
    {
        // center it
        float tiles_w = SCREEN_W / 16; //30
        float tiles_h = SCREEN_H / 16; //17

//        x -= (tiles_w / 2) - 1;
//        y -= ceil(tiles_h / 2) - 1;
//
        //init pos is:
        // -14, -8
    }

    float x = 0;
    float y = 0;
};


#endif //SPELUNKYPSP_CAMERA_H
