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

    bool re_batching_needed();
    void batch_vertices();
    void render() const;
    void load_texture_uv();

private:

    int32_t _last_camera_x_in_tiles = 0;
    int32_t _last_camera_y_in_tiles = 0;

    RenderTile _tiles[static_cast<uint32_t>(MapTileType::_SIZE)];
    GLuint _tilesheet = 0;

    struct Vertex
    {
        int16_t x;
        int16_t y;
        float u;
        float v;
    };

    struct
    {
        std::vector<int16_t > xyz;
        std::vector<GLfloat> uv;
        std::vector<std::int16_t> indices;

        std::vector<Vertex> merged;
        std::size_t tile_counter = 0;
    } _render_batch;

    static LevelRenderer* _level_renderer;
};
