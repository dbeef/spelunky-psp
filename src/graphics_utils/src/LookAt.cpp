//
// Created by dbeef on 2/24/20.
//

#include <graphics_utils/LookAt.hpp>
#include <graphics_utils/DebugGlCall.hpp>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "glad/glad.h"

void graphics_utils::look_at(const Camera &camera)
{
    const glm::vec3 eye = {-camera.getX(), camera.getY(), -1.0f};
    const glm::vec3 center = {-camera.getX(), camera.getY(), 0.0f};
    const glm::vec3 up = {0.0f, 1.0f, 0.0f};

    const auto M = glm::lookAt(eye, center, up);

    DebugGlCall(glMultMatrixf(glm::value_ptr(M)));
    DebugGlCall(glTranslatef(-eye[0], -eye[1], -eye[2]));
}
