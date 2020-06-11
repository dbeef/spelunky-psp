#pragma once

#include <memory>
// Note: For both cameras coordinate system goes as following:
//
//   ------ +x
//   |
//   |
//   +y
//

class Viewport;

class ModelViewCamera
{
public:

    ModelViewCamera(std::shared_ptr<Viewport>);

    void update_gl_modelview_matrix() const;
    void update_gl_projection_matrix() const;

    inline float get_x() const { return _x; }
    inline float get_y() const { return _y; }

    inline void set_x(float x) { _x = x; round_position_x(); }
    inline void set_y(float y) { _y = y; round_position_y(); }

    inline void set_x_not_rounded(float x) { _x = x; }
    inline void set_y_not_rounded(float y) { _y = y; }

    void adjust_to_bounding_box(float x, float y);
    void adjust_to_level_boundaries(float level_width, float level_height);

    void calculate_coefficients();

private:

    void round_position_x();
    void round_position_y();
    
    std::shared_ptr<Viewport> _viewport;

    float _x = 0;
    float _y = 0;

    const float _bounding_x;
    const float _bounding_y;
    const float _bounding_x_half;
    const float _bounding_y_half;

    float _projection_coefficient;
    float _screen_width_tiles;
    float _screen_height_tiles;
};
