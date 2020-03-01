#pragma once

#include <cstdint>

class Camera
{
public:

    void update_gl_modelview_matrix();
    void update_gl_projection_matrix() const;

    static Camera& instance();
    static void init();
    static void dispose();

    inline float getX() const { return _x; }
    inline float getY() const { return _y; }

    inline void setX(float x) { _dirty = true; _x = x; }
    inline void setY(float y) { _dirty = true; _y = y; }

private:

    bool _dirty = false;

    float _x = 0;
    float _y = 0;

    static Camera* _camera;
};
