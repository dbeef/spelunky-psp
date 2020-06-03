#include <cassert>
#include <glad/glad.h>
#include "video/Context.hpp"
#include "graphics_utils/DebugGlCall.hpp"
#include "ScreenSpaceCamera.hpp"

ScreenSpaceCamera* ScreenSpaceCamera::_camera = nullptr;

void ScreenSpaceCamera::update_gl_modelview_matrix()
{
    DebugGlCall(glMatrixMode(GL_MODELVIEW));
    DebugGlCall(glLoadIdentity());
}

void ScreenSpaceCamera::update_gl_projection_matrix() const
{
    const auto& video = Video::instance();
    DebugGlCall(glViewport(0, 0, (GLsizei) (video.get_window_width()), (GLsizei) (video.get_window_height())));
    DebugGlCall(glMatrixMode(GL_PROJECTION));
    DebugGlCall(glLoadIdentity());

    static const GLdouble near = -100;
    static const GLdouble far = 100;

    DebugGlCall(glOrtho(-1 * _projection_coefficient * video.get_aspect(), // How much tiles will fit on half of the screen width.
                        1 * _projection_coefficient * video.get_aspect(),
                        1 * _projection_coefficient, // How much tiles will fit on half of the screen height.
                        -1 * _projection_coefficient,
                        near,
                        far));
}

void ScreenSpaceCamera::dispose()
{
    assert(_camera);
    delete _camera;
    _camera = nullptr;

}

void ScreenSpaceCamera::init()
{
    assert(!_camera);
    _camera = new ScreenSpaceCamera();
}

ScreenSpaceCamera &ScreenSpaceCamera::instance()
{
    assert(_camera);
    return *_camera;
}

void ScreenSpaceCamera::calculate_coefficients()
{
    const auto& video = Video::instance();
    _projection_coefficient = (Video::instance().get_window_width() / video.get_aspect()) / 2.0f;
}

