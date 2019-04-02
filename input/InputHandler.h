//
// Created by dbeef on 2/23/19.
//

#ifndef SPELUNKY_PSP_INPUTHANDLER_H
#define SPELUNKY_PSP_INPUTHANDLER_H

#include "../camera/Camera.h"

#include <stdlib.h>
#include <GL/glut.h>

#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <stdlib.h>
#include <pspctrl.h>
#include <pspgu.h>
#include <pspgum.h>
#include <stdio.h>
#include <pspctrl.h>

class InputHandler {

public:

    Camera* _camera;

    SceCtrlData pad{};

    InputHandler(Camera* camera) : _camera(camera) {
        sceCtrlSetSamplingCycle(0);
        sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
    }

    void handle() {

        pspDebugScreenSetXY(0, 2);
        sceCtrlReadBufferPositive(&pad, 1);

//        //printf("Analog X = %d ", pad.Lx);
//        //printf("Analog Y = %d \n", pad.Ly);

        if (pad.Buttons != 0) {
            if (pad.Buttons & PSP_CTRL_SQUARE) {
                //printf("Square pressed \n");
                _camera->x -= 2.0f/16;
            }
            if (pad.Buttons & PSP_CTRL_TRIANGLE) {
                //printf("Triangle pressed \n");
                _camera->y += 2.0f/16;
            }
            if (pad.Buttons & PSP_CTRL_CIRCLE) {
                //printf("Cicle pressed \n");
                _camera->x += 2.0f/16;
            }
            if (pad.Buttons & PSP_CTRL_CROSS) {
                //printf("Cross pressed \n");
                _camera->y -= 2.0f/16;
            }

            if (pad.Buttons & PSP_CTRL_UP) {
                //printf("Up pressed \n");
            }
            if (pad.Buttons & PSP_CTRL_DOWN) {
                //printf("Down pressed \n");
            }
            if (pad.Buttons & PSP_CTRL_LEFT) {
                //printf("Left pressed \n");
            }
            if (pad.Buttons & PSP_CTRL_RIGHT) {
                //printf("Right pressed \n");
            }

            if (pad.Buttons & PSP_CTRL_START) {
                //printf("Start pressed \n");
            }
            if (pad.Buttons & PSP_CTRL_SELECT) {
                //printf("Select pressed \n");
            }
            if (pad.Buttons & PSP_CTRL_LTRIGGER) {
                //printf("L-trigger pressed \n");
            }
            if (pad.Buttons & PSP_CTRL_RTRIGGER) {
                //printf("R-trigger pressed \n");
            }
        }


    }

};


#endif //SPELUNKY_PSP_INPUTHANDLER_H
