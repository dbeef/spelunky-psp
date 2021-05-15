#include "Input.hpp"

#include <pspctrl.h>

const char* Input::get_pause_binding_msg()
{
    return "SELECT";
}

const char* Input::get_death_request_binding_msg()
{
    return "L";
}

const char* Input::get_quit_request_binding_msg()
{
    return "R";
}

const char* Input::get_accept_transaction_binding_msg()
{
    return "START";
}

const char* Input::get_scores_request_binding_msg()
{
    return "START";
}

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

    _toggles.up.feed(pad.Buttons & PSP_CTRL_UP);
    _toggles.down.feed(pad.Buttons & PSP_CTRL_DOWN);
    _toggles.left.feed(pad.Buttons & PSP_CTRL_LEFT);
    _toggles.right.feed(pad.Buttons & PSP_CTRL_RIGHT);
    _toggles.jumping.feed(pad.Buttons & PSP_CTRL_CROSS);
    _toggles.ducking.feed(pad.Buttons & PSP_CTRL_CIRCLE);
    _toggles.death_requested.feed(pad.Buttons & PSP_CTRL_LTRIGGER);
    _toggles.quit_requested.feed(pad.Buttons & PSP_CTRL_RTRIGGER);
    _toggles.running_fast.feed(pad.Buttons & PSP_CTRL_LTRIGGER);
    _toggles.throwing.feed(pad.Buttons & PSP_CTRL_RTRIGGER);
    _toggles.paused.feed(pad.Buttons & PSP_CTRL_SELECT);
    _toggles.out_bomb.feed(pad.Buttons & PSP_CTRL_TRIANGLE);
    _toggles.out_rope.feed(pad.Buttons & PSP_CTRL_SQUARE);
    _toggles.purchase.feed(pad.Buttons & PSP_CTRL_START);

    fill_input_events();
}
