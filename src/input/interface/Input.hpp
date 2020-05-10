#pragma once

class Input
{
public:

    static Input& instance();
    static void init();
    static void dispose();

    void poll();

    inline bool isExit() const { return _exit; }

    inline bool left() const { return _left; }
    inline bool right() const { return _right; }
    inline bool up() const { return _up; }
    inline bool down() const { return _down; }

    inline bool square() const { return _square; }
    inline bool circle() const { return _circle; }
    inline bool triangle() const { return _triangle; }
    inline bool cross() const { return _cross; }

private:

    bool _exit = false;

    bool _left = false;
    bool _right = false;
    bool _up = false;
    bool _down = false;

    bool _square = false;
    bool _circle = false;
    bool _triangle = false;
    bool _cross = false;

    static Input* _input;
};
