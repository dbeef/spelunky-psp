#pragma once

#include <cstdint>

class Camera
{
public:

    Camera();

    void update_gl();

    static Camera& instance();
    static void init();
    static void dispose();

    inline float getX() const { return _x; }
    inline float getY() const { return _y; }

    inline void setX(float x) { _dirty = true; _x = x; }
    inline void setY(float y) { _dirty = true; _y = y; }

private:

    bool _dirty = false;

    float _x;
    float _y;

    static Camera* _camera;
};
