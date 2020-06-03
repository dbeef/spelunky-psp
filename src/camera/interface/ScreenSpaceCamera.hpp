#pragma once

class ScreenSpaceCamera
{
public:

    void update_gl_modelview_matrix();
    void update_gl_projection_matrix() const;

    void calculate_coefficients();

private:

    float _projection_coefficient;
};
