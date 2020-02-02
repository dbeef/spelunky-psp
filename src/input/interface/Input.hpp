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

    void poll();

    inline bool isExit() const { return _exit; }
    inline bool left() const { return _left; }
    inline bool right() const { return _right; }
    inline bool up() const { return _up; }
    inline bool down() const { return _down; }

private:

    bool _exit;
    bool _left;
    bool _right;
    bool _up;
    bool _down;

    static Input* _input;
};
