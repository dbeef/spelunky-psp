#include "graphics_utils/DebugGlCall.hpp"
#include "viewport/Viewport.hpp"
#include "glad/glad.h"
#include "graphics_utils/LookAt.hpp"
#include "ModelViewCamera.hpp"

#include "glm/glm.hpp"
#include <cstdlib>
#include <cmath>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

ModelViewCamera::ModelViewCamera(std::shared_ptr<Viewport> viewport)
    : _viewport(std::move(viewport))
    , _bounding_x(0.0f)
    , _bounding_y(0.0f)
    , _bounding_x_half(_bounding_x / 2)
    , _bounding_y_half(_bounding_y / 2)
{ }

void ModelViewCamera::update_gl_modelview_matrix() const
{
    graphics_utils::look_at(_x, _y);
}

void ModelViewCamera::update_gl_projection_matrix() const
{
    DebugGlCall(glViewport(0, 0, (GLsizei) (_viewport->get_width_pixels()), (GLsizei) (_viewport->get_height_pixels())));
    DebugGlCall(glMatrixMode(GL_PROJECTION));
    DebugGlCall(glLoadIdentity());

    static const GLdouble near = 100;
    static const GLdouble far = 0.0;

//    const auto projection_matrix = glm::perspective<float>(glm::radians<float>(-80),
//                                                    _viewport->get_aspect(),
//                                                    near,
//                                                    far);
//
//    DebugGlCall(glLoadMatrixf(glm::value_ptr(projection_matrix)));

    DebugGlCall(glOrtho(-1 * _projection_coefficient * _viewport->get_aspect(), // How much tiles will fit on half of the screen width.
                         1 * _projection_coefficient * _viewport->get_aspect(),
                         1 * _projection_coefficient, // How much tiles will fit on half of the screen height.
                        -1 * _projection_coefficient,
                         near,
                         far));
}

void ModelViewCamera::calculate_coefficients()
{
    _projection_coefficient = _viewport->calculate_coefficient_world_units();
    _screen_width_tiles = _viewport->get_width_world_units();
    _screen_height_tiles = _viewport->get_height_world_units();
}

// rounding the values to 1 decimal point
// to avoid vertical screen-tearing like artifacts
void ModelViewCamera::round_position_x() { _x = ((10.f * _x + 0.5f) / 10); }
void ModelViewCamera::round_position_y() { _y = ((10.f * _y + 0.5f) / 10); }

// FIXME: These methods should not be in competence of the camera implementation - move them out.

void ModelViewCamera::adjust_to_bounding_box(float x, float y)
{
    // For now stick camera to the main dude:

//    _x = x;
//    _y = y;
//
//    round_position_x();
//    round_position_y();

    float dx = (x / 2) - _x;
    float dy = (y / 2) - _y;

    if (std::abs(dx) > _bounding_x_half)
    {
        _x += dx + (dx > 0.f ? -_bounding_x_half : _bounding_x_half);
        round_position_x();
    }

    if (std::abs(dy) > _bounding_y_half)
    {
        _y += dy + (dy > 0.f ? -_bounding_y_half : _bounding_y_half);
        round_position_y();
    }
}

void ModelViewCamera::adjust_to_level_boundaries(float level_width, float level_height)
{
    // With assumption that the level starts on [0,0]:

    float x_tile_space = _x * 2;
    float y_tile_space = _y * 2;

    const float half_screen_width_tiles = _screen_width_tiles / 2.0f;
    const float half_screen_height_tiles = _screen_height_tiles / 2.0f;

    if (x_tile_space + half_screen_width_tiles > level_width)
    {
        float x_camera_space = (level_width - half_screen_width_tiles) / 2.0f;
        _x = x_camera_space;
    }

    if (x_tile_space - half_screen_width_tiles < 0.0f)
    {
        float x_camera_space = (half_screen_width_tiles) / 2.0f;
        _x = x_camera_space;
    }

    if (y_tile_space + half_screen_height_tiles > level_height)
    {
        float y_camera_space = (level_height - half_screen_height_tiles) / 2.0f;
        _y = y_camera_space;
    }

    if (y_tile_space - half_screen_height_tiles < 0.0f)
    {
        float y_camera_space = (half_screen_height_tiles) / 2.0f;
        _y = y_camera_space;
    }
}
