#pragma once

// Note: For both cameras coordinate system goes as following:
//
//   ------ +x
//   |
//   |
//   +y
//

class ScreenSpaceCamera
{
public:

    void update_gl_modelview_matrix() const;
    void update_gl_projection_matrix() const;

    void calculate_coefficients();

private:

    float _projection_coefficient;
};
