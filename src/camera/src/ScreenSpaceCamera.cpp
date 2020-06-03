#include <glad/glad.h>

#include "ScreenSpaceCamera.hpp"
#include "video/Context.hpp"
#include "graphics_utils/DebugGlCall.hpp"
#include "graphics_utils/LookAt.hpp"

void ScreenSpaceCamera::update_gl_modelview_matrix() const
{
    const auto& video = Video::instance();

    // Moving camera, so the [0,0] position would be on the left-lower corner of the screen:

    const float screen_center_x = video.get_window_width() / 2.0f;
    const float screen_center_y = video.get_window_height() / 2.0f;

    const float screen_center_camera_space_x = screen_center_x / 2.0f;
    const float screen_center_camera_space_y = screen_center_y / 2.0f;

    graphics_utils::look_at(screen_center_camera_space_x, -screen_center_camera_space_y);
}

void ScreenSpaceCamera::update_gl_projection_matrix() const
{
    const auto& video = Video::instance();
    DebugGlCall(glViewport(0, 0, (GLsizei) (video.get_window_width()), (GLsizei) (video.get_window_height())));
    DebugGlCall(glMatrixMode(GL_PROJECTION));
    DebugGlCall(glLoadIdentity());

    static const GLdouble near = -100;
    static const GLdouble far = 100;

    DebugGlCall(glOrtho(-1 * _projection_coefficient * video.get_aspect(), // How much pixels will fit on half of the screen width.
                        1 * _projection_coefficient * video.get_aspect(),
                        1 * _projection_coefficient, // How much pixels will fit on half of the screen height.
                        -1 * _projection_coefficient,
                        near,
                        far));
}

void ScreenSpaceCamera::calculate_coefficients()
{
    const auto& video = Video::instance();
    _projection_coefficient = (Video::instance().get_window_width() / video.get_aspect()) / 2.0f;
}


