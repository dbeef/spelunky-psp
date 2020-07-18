#pragma once

class Input
{
public:

    static Input& instance();
    static void init();
    static void dispose();

    static const char* get_pause_binding_msg();
    static const char* get_death_request_binding_msg();
    static const char* get_quit_request_binding_msg();

    void poll();

    inline bool paused() const { return _paused; }
    inline bool death_requested() const { return _death_requested; }
    inline bool quit_requested() const { return _quit_requested; }

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
    bool _death_requested = false;
    bool _quit_requested = false;

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
