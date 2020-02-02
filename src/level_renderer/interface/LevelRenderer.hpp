#pragma once

#include "glad/glad.h"
#include <vector>

class LevelRenderer
{
public:

    static LevelRenderer& instance();
    static void init();
    static void dispose();

    void render();
    void set_projection_matrix();
    void load_textures();

private:

    std::vector<GLfloat> _batch_xyz;
    std::vector<GLfloat> _batch_uv;
    GLuint _tilesheet = 0;
    static LevelRenderer* _level_renderer;
};
