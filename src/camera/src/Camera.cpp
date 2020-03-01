#include <cassert>
#include <graphics_utils/DebugGlCall.hpp>

#include "glad/glad.h"
#include "graphics_utils/LookAt.hpp"
#include "Camera.hpp"

Camera* Camera::_camera = nullptr;

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

Camera::Camera() : _x(0), _y(0) {}

void Camera::update_gl()
{
    if (_dirty)
    {
        DebugGlCall(glMatrixMode(GL_MODELVIEW));
        DebugGlCall(glLoadIdentity());
        DebugGlCall(glTranslatef(0, 0, 0));
        DebugGlCall(graphics_utils::look_at(*this));

        _dirty = false;
    }
}
