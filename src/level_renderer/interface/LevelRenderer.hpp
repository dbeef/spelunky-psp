#pragma once

#include "glad/glad.h"
#include <vector>

class LevelRenderer
{
public:

    static LevelRenderer& instance();
    static void init();
    static void dispose();

    void set_texture_pointer_to_tile(int type);
    void render();
    void set_projection_matrix();

    void load_textures();

private:

    float coordinates[4][2];
    std::vector<GLfloat> _batch_xyz;
    std::vector<GLfloat> _batch_uv;
    GLuint _tilesheet = 0;
    static LevelRenderer* _level_renderer;
};
