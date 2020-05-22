#include <cassert>

#include "graphics_utils/DebugGlCall.hpp"
#include "video/Context.hpp"
#include "glad/glad.h"
#include "graphics_utils/LookAt.hpp"
#include "Camera.hpp"

namespace
{
    // Returns coefficient to glOrtho of such value,
    // so screen with dimensions of screen_width_tiles would entirely fit on the screen.
    float calculate_coefficient(std::size_t screen_width_tiles)
    {
        const auto& video = Video::instance();
        return (screen_width_tiles / video.get_aspect()) / 2.0f;
    }

}

Camera *Camera::_camera = nullptr;

Camera &Camera::instance()
{
    assert(_camera);
    return *_camera;
}

void Camera::init()
{
    assert(!_camera);
    _camera = new Camera();
}

void Camera::dispose()
{
    assert(_camera);
    delete _camera;
    _camera = nullptr;
}

void Camera::update_gl_modelview_matrix()
{
    if (_dirty)
    {
        graphics_utils::look_at(_x, _y);
        _dirty = false;
    }
}

void Camera::update_gl_projection_matrix() const
{
    const auto& video = Video::instance();

    DebugGlCall(glViewport(0, 0, (float) (video.get_window_width()), (float) (video.get_window_height())));
    DebugGlCall(glMatrixMode(GL_PROJECTION));
    DebugGlCall(glLoadIdentity());

    static const float coeff = calculate_coefficient(20);
    static const GLdouble near = -100;
    static const GLdouble far = 100;

    DebugGlCall(glOrtho(-1 * coeff * video.get_aspect(), // How much tiles will fit on half of the screen width.
                         1 * coeff * video.get_aspect(),
                         1 * coeff, // How much tiles will fit on half of the screen height.
                        -1 * coeff,
                         near,
                         far));
}
