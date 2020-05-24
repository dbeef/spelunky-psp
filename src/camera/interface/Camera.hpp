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

    inline void set_x(float x) { _dirty = true; _x = x; round_position_x(); }
    inline void set_y(float y) { _dirty = true; _y = y; round_position_y(); }

    inline void set_x_not_rounded(float x) { _dirty = true; _x = x; }
    inline void set_y_not_rounded(float y) { _dirty = true; _y = y; }

    void adjust_to_bounding_box(float x, float y);
    void adjust_to_level_boundaries(float level_width, float level_height);

    void calculate_coefficients();

private:
    explicit Camera();

    void round_position_x();
    void round_position_y();

    bool _dirty = false;

    float _x = 0;
    float _y = 0;

    const float _bounding_x;
    const float _bounding_y;
    const float _bounding_x_half;
    const float _bounding_y_half;

    float _projection_coefficient;
    float _screen_width_tiles;
    float _screen_height_tiles;

    static Camera* _camera;
};
