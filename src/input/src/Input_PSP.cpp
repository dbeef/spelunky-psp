#include "Input.hpp"

#include <pspctrl.h>

void Input::poll()
{
    static bool set_up = false;

    if (!set_up)
    {
        sceCtrlSetSamplingCycle(0);
        sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
        set_up = true;
    }

    SceCtrlData pad{};
    sceCtrlReadBufferPositive(&pad, 1);

    _up = pad.Buttons & PSP_CTRL_UP;
    _down = pad.Buttons & PSP_CTRL_DOWN;
    _left = pad.Buttons & PSP_CTRL_LEFT;
    _right = pad.Buttons & PSP_CTRL_RIGHT;

    _triangle = pad.Buttons & PSP_CTRL_TRIANGLE;
    _cross = pad.Buttons & PSP_CTRL_CROSS;
    _square = pad.Buttons & PSP_CTRL_SQUARE;
    _circle = pad.Buttons & PSP_CTRL_CIRCLE;

    _bumper_l = pad.Buttons & PSP_CTRL_LTRIGGER;
    _bumper_r = pad.Buttons & PSP_CTRL_RTRIGGER;
}
