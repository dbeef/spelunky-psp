#pragma once

class ScreenSpaceCamera
{
public:

    void update_gl_modelview_matrix();
    void update_gl_projection_matrix() const;

    ScreenSpaceCamera(ScreenSpaceCamera&) = delete;
    ScreenSpaceCamera(ScreenSpaceCamera&&) = delete;

    static ScreenSpaceCamera& instance();
    static void init();
    static void dispose();

    void calculate_coefficients();

private:

    explicit ScreenSpaceCamera() = default;

    float _projection_coefficient;
    static ScreenSpaceCamera* _camera;
};
