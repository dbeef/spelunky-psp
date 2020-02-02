#pragma once

#include "glad/glad.h"

class LevelRenderer
{
public:

    static LevelRenderer& instance();
    static void init();
    static void dispose();

    void load_textures();

private:

    GLuint _tilesheet = 0;
    static LevelRenderer* _level_renderer;
};
