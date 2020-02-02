#include <cassert>

#include "Camera.hpp"

namespace
{
    const std::uint16_t SCREEN_WIDTH = 480;
    const std::uint16_t SCREEN_HEIGHT = 272;
}

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
