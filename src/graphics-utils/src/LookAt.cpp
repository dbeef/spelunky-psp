//
// Created by dbeef on 2/24/20.
//

#include <graphics_utils/LookAt.hpp>
#include <graphics_utils/DebugGlCall.hpp>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "glad/glad.h"
#include "Input.hpp"

void graphics_utils::look_at_screen_space(float x, float y)
{
    DebugGlCall(glMatrixMode(GL_MODELVIEW));
    DebugGlCall(glLoadIdentity());
    DebugGlCall(glTranslatef(0, 0, 0));

    const glm::vec3 eye = {x, y, 1.0f};
    const glm::vec3 center = {x, y, 0.0f};
    const glm::vec3 up = {0.0f, 1.0f, 0.0f};

    const auto M = glm::lookAt(eye, center, up);

    DebugGlCall(glMultMatrixf(glm::value_ptr(M)));
    DebugGlCall(glTranslatef(-eye[0], -eye[1], -eye[2]));
}

void graphics_utils::look_at(float x, float y)
{
    auto& input = Input::instance();
    auto& accelerometer = input.get_accelerometer_input();
    
    //accelerometer.x = 0.0f;
    //accelerometer.y = 0.0f;
    //accelerometer.z = 0.0f;

    DebugGlCall(glMatrixMode(GL_MODELVIEW));
    DebugGlCall(glLoadIdentity());
    DebugGlCall(glTranslatef(0.0f, 0.0f, 10.0f));

    glm::vec4 eye = glm::vec4{x + accelerometer.x,
                                                    y + accelerometer.y,
                                                    8.0f + accelerometer.z,
                                                    1.0f};

    const glm::vec3 center = {x, y, 0.0f};
    const glm::vec3 up = {0.0f, 1.0f, 0.0f};
    const auto M = glm::lookAt(glm::vec3{eye.x, eye.y, eye.z}, center, up);

    DebugGlCall(glMultMatrixf(glm::value_ptr(M)));
    DebugGlCall(glTranslatef(-eye[0], -eye[1], -eye[2]));
}
