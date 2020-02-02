#pragma once

#include <cstdint>
#include <SDL/SDL_events.h>

class Input
{
public:

    Input();

    static Input& instance();
    static void init();
    static void dispose();

    void process(SDL_Event&);

private:

    bool _left;
    bool _right;
    bool _up;
    bool _down;

    static Input* _Input;
};
