#pragma once

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

    inline bool square() const { return _square; }
    inline bool circle() const { return _circle; }
    inline bool triangle() const { return _triangle; }
    inline bool cross() const { return _cross; }

private:

    bool _exit;

    bool _left;
    bool _right;
    bool _up;
    bool _down;

    bool _square;
    bool _circle;
    bool _triangle;
    bool _cross;

    static Input* _input;
};
