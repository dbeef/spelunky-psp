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

    _jumping = pad.Buttons & PSP_CTRL_CROSS;
    _ducking = pad.Buttons & PSP_CTRL_CIRCLE;

    _running_fast = pad.Buttons & PSP_CTRL_LTRIGGER;
    _throwing = pad.Buttons & PSP_CTRL_RTRIGGER;
}
