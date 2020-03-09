#include <cassert>

#include "graphics_utils/DebugGlCall.hpp"
#include "video/Context.hpp"
#include "glad/glad.h"
#include "graphics_utils/LookAt.hpp"
#include "Camera.hpp"

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
        graphics_utils::look_at(*this);
        _dirty = false;
    }
}

void Camera::update_gl_projection_matrix() const
{
    DebugGlCall(glViewport(0, 0, (float) (Video::get_window_width()), (float) (Video::get_window_height())));
    DebugGlCall(glMatrixMode(GL_PROJECTION));
    DebugGlCall(glLoadIdentity());

    const float coeff = 6.0f;

    DebugGlCall(glOrtho(-1 * coeff * Video::get_aspect(),
                         1 * coeff * Video::get_aspect(),
                         1 * coeff,
                        -1 * coeff,
                        -1 * coeff,
                         1 * coeff));
}
