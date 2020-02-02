#include <cassert>

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
}

Camera::Camera() : _x(0), _y(0) {}
