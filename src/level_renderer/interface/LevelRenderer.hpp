#pragma once

#include "glad/glad.h"
#include "MapTileType.hpp"
#include "RenderTile.hpp"

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

    RenderTile _tiles[static_cast<uint32_t>(MapTileType::_SIZE)];
    GLuint _tilesheet = 0;

    std::vector<GLfloat> _batch_xyz;
    std::vector<GLfloat> _batch_uv;
    std::vector<GLuint> _batch_indices;

    std::size_t _tile_counter = 0;

    static LevelRenderer* _level_renderer;
};
