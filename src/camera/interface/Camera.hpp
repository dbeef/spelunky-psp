#pragma once

#include <cstdint>

class Camera
{
public:

    Camera();

    static Camera& instance();
    static void init();
    static void dispose();

    inline float getX() { return _x; }
    inline float getY() { return _y; }

private:

    float _x;
    float _y;

    static Camera* _camera;
};
