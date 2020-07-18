#pragma once

class Input
{
public:

    static Input& instance();
    static void init();
    static void dispose();

    void poll();

    inline bool paused() const { return _paused; }

    inline bool left() const { return _left; }
    inline bool right() const { return _right; }
    inline bool up() const { return _up; }
    inline bool down() const { return _down; }

    inline bool jumping() const { return _jumping; }
    inline bool ducking() const { return _ducking; }

    inline bool running_fast() const { return _running_fast; }
    inline bool throwing() const { return _throwing; }

private:

    bool _paused = false;

    bool _left = false;
    bool _right = false;
    bool _up = false;
    bool _down = false;

    bool _jumping = false;
    bool _ducking = false;

    bool _running_fast = false;
    bool _throwing = false;

    static Input* _input;
};
