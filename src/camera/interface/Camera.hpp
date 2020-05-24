#pragma once

class Camera
{
public:

    void update_gl_modelview_matrix();
    void update_gl_projection_matrix() const;
    Camera(Camera&) = delete;
    Camera(Camera&&) = delete;
    static Camera& instance();
    static void init();
    static void dispose();

    inline float get_x() const { return _x; }
    inline float get_y() const { return _y; }

    inline void set_x(float x) { _dirty = true; _x = x; }
    inline void set_y(float y) { _dirty = true; _y = y; }

    void adjust_to_bounding_box(float x, float y);
private:
    explicit Camera();

    bool _dirty = false;

    float _x = 0;
    float _y = 0;
    
    const float _bounding_x;
    const float _bounding_y;
    const float _bounding_x_half;
    const float _bounding_y_half;

    static Camera* _camera;
};
