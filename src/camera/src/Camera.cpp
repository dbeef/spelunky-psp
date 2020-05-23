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

Camera::Camera()
    : _bounding_x(2.f)
    , _bounding_y(1.3f)
    , _bounding_x_half(_bounding_x / 2)
    , _bounding_y_half(_bounding_y / 2)
{ }

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
        // rounding the values to 1 decimal point
        // to avoid vertical screen-tearing like artifacts
        auto rounded_x = (10.f * _x + .5f) / 10;
        auto rounded_y = (10.f * _y + .5f) / 10;

        graphics_utils::look_at(rounded_x, rounded_y);
        _dirty = false;
    }
}

void Camera::update_gl_projection_matrix() const
{
    const auto& video = Video::instance();
    DebugGlCall(glViewport(0, 0, (GLsizei) (video.get_window_width()), (GLsizei) (video.get_window_height())));
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

void Camera::adjust_to_bounding_box(float x, float y)
{
    auto dx = (x / 2) - _x;
    auto dy = (y / 2) - _y;

    if (std::abs(dx) > _bounding_x_half)
    {
        _x+= dx + (dx > 0.f ? -_bounding_x_half : _bounding_x_half);
        _dirty = true;
    }
    
    if (std::abs(dy) > _bounding_y_half)
    {
        _y+= dy + (dy > 0.f ? -_bounding_y_half : _bounding_y_half);
        _dirty = true;
    }
}
