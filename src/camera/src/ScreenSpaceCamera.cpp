#include <cassert>
#include <glad/glad.h>

#include "ScreenSpaceCamera.hpp"
#include "viewport/Viewport.hpp"
#include "graphics_utils/DebugGlCall.hpp"
#include "graphics_utils/LookAt.hpp"


ScreenSpaceCamera::ScreenSpaceCamera(std::shared_ptr<Viewport> viewport, CoordinateType coordinate_type)
    : _viewport(std::move(viewport))
    , _coordinate_type(coordinate_type)
{}

void ScreenSpaceCamera::update_gl_modelview_matrix() const
{
    // Moving camera, so the [0,0] position would be on the left-lower corner of the screen:

    float screen_center_x;
    float screen_center_y;

    switch (_coordinate_type)
    {
        case CoordinateType::WORLD_UNITS:
        {
            screen_center_x = _viewport->get_width_world_units() / 2.0f;
            screen_center_y = _viewport->get_height_world_units() / 2.0f;
            break;
        }
        case CoordinateType::PIXELS:
        {
            screen_center_x = _viewport->get_width_pixels() / 2.0f;
            screen_center_y = _viewport->get_height_pixels() / 2.0f;
            break;
        }
        default:
        {
            assert(false);
        }
    }

    const float screen_center_camera_space_x = screen_center_x / 2.0f;
    const float screen_center_camera_space_y = screen_center_y / 2.0f;

    graphics_utils::look_at(screen_center_camera_space_x, screen_center_camera_space_y);
}

void ScreenSpaceCamera::update_gl_projection_matrix() const
{
    DebugGlCall(glViewport(0, 0, (GLsizei) (_viewport->get_width_pixels()), (GLsizei) (_viewport->get_height_pixels())));
    DebugGlCall(glMatrixMode(GL_PROJECTION));
    DebugGlCall(glLoadIdentity());

    static const GLdouble near = -100;
    static const GLdouble far = 100;

    DebugGlCall(glOrtho(-1 * _projection_coefficient * _viewport->get_aspect(), // How much pixels or world units will fit on half of the screen width.
                        1 * _projection_coefficient * _viewport->get_aspect(),
                        1 * _projection_coefficient, // How much pixels or world units will fit on half of the screen height.
                        -1 * _projection_coefficient,
                        near,
                        far));
}

void ScreenSpaceCamera::calculate_coefficients()
{
    switch (_coordinate_type)
    {
        case CoordinateType::WORLD_UNITS:
        {
            _projection_coefficient = _viewport->calculate_coefficient_world_units();
            break;
        }
        case CoordinateType::PIXELS:
        {
            _projection_coefficient = _viewport->calculate_coefficient_pixels();
            break;
        }
        default:
        {
            assert(false);
        }
    }
}
