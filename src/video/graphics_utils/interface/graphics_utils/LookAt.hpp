//
// Created by dbeef on 2/2/20.
//

#ifndef RESOURCE_COMPILER_LOOKAT_HPP
#define RESOURCE_COMPILER_LOOKAT_HPP

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "glad/glad.h"
#include "DebugGlCall.hpp"

#include "Camera.hpp"

namespace graphics_utils
{
    void look_at(Camera &camera)
    {
        glm::vec3 eye = {-camera.getX(), camera.getY(), -1.0f};
        glm::vec3 center = {-camera.getX(), camera.getY(), 0.0f};
        glm::vec3 up = {0.0f, 1.0f, 0.0f};

        auto M = glm::lookAt(eye, center, up);

        DebugGlCall(glMultMatrixf(glm::value_ptr(M)));
        DebugGlCall(glTranslatef(-eye[0], -eye[1], -eye[2]));
    }
}

#endif //RESOURCE_COMPILER_LOOKAT_HPP
