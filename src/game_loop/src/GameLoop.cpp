//
// Created by dbeef on 2/5/20.
//

#include "Camera.hpp"
#include "Input.hpp"
#include "GameLoop.hpp"
#include "glad/glad.h"
#include "graphics_utils/DebugGlCall.hpp"
#include "LevelRenderer.hpp"

namespace
{
    void handle_input()
    {
        Input& input = Input::instance();
        Camera& camera = Camera::instance();

        if (input.left())
        {
            camera.setX(camera.getX() - 0.1f);
        }
        if (input.right())
        {
            camera.setX(camera.getX() + 0.1f);
        }
        if (input.up())
        {
            camera.setY(camera.getY() - 0.1f);
        }
        if (input.down())
        {
            camera.setY(camera.getY() + 0.1f);
        }
    }
}

std::function<void()>& GameLoop::get()
{
    return _loop;
}

GameLoop::GameLoop()
{
    _loop = []()
    {
        DebugGlCall(glClear(GL_COLOR_BUFFER_BIT));
        LevelRenderer::instance().render();
        handle_input();
    };
}
