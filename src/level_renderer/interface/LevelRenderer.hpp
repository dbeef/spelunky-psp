#pragma once

#include "glad/glad.h"

class LevelRenderer
{
public:

    static LevelRenderer& instance();
    static void init();
    static void dispose();

    void set_texture_pointer_to_tile(int type);
    void write_tiles_to_map();
    void render();

    void load_textures();

private:

    float coordinates[4][2];
    std::vector<GLfloat> _batch;
    GLuint _tilesheet = 0;
    static LevelRenderer* _level_renderer;
};
