#include "LevelRenderer.hpp"
#include "LevelGenerator.hpp"
#include "video/Context.hpp"

#include "glad/glad.h"
#include "logger/log.h"

#include "graphics_utils/LookAt.hpp"
#include "graphics_utils/CreateTexture.hpp"
#include "graphics_utils/DebugGlCall.hpp"

namespace
{
    namespace tilesheet
    {
        #include "gfx_cavebg.png.h"
    }

    void set_texture_pointer_to_tile(int type, float coordinates[4][2]);
}

LevelRenderer* LevelRenderer::_level_renderer = nullptr;

LevelRenderer &LevelRenderer::instance()
{
    assert(_level_renderer);
    return *_level_renderer;
}

void LevelRenderer::init()
{
    assert(!_level_renderer);
    _level_renderer = new LevelRenderer();
}

void LevelRenderer::dispose()
{
    assert(_level_renderer);
    delete _level_renderer;
    _level_renderer = nullptr;
}

void LevelRenderer::load_textures()
{
    log_info("Loading tilesheet.");

    _tilesheet = graphics_utils::createTexture(tilesheet::data, sizeof(tilesheet::data));
    assert(_tilesheet);

    log_info("Done loading tilesheet.");
}

void LevelRenderer::set_projection_matrix()
{
    auto& level = LevelGenerator::instance();
    auto& camera = Camera::instance();

    DebugGlCall(glViewport(0, 0, Video::getWindowWidth(), Video::getWindowHeight()));
    DebugGlCall(glMatrixMode(GL_PROJECTION));
    DebugGlCall(glLoadIdentity());
    float aspect_ratio = static_cast<float>(Video::getWindowWidth()) / Video::getWindowHeight();

    DebugGlCall(glOrtho(-8 * aspect_ratio, 8 * aspect_ratio, 8, -8, -8, 8));
}

// FIXME: This is very poorly written rendering function; refactor needed.
void LevelRenderer::render()
{
    auto& level = LevelGenerator::instance();
    auto& camera = Camera::instance();

    // In opengl:
    // The top-left corner will be at (-1, 1).
    // In spelunkyds:
    // top-left corner is 0,0 and right-lower is 1, 1
    DebugGlCall(glBindTexture(GL_TEXTURE_2D, _tilesheet));

// first optimization thought -> go through all tiles of the same type, upload once & render all
// it's 32 x 32 uploads per frame
// second thought - cache a few most used textures? won't make much of a difference if there are 4-8 slots...
// 4th thought -> render to framebuffer first?
// limit rendered tiles to only those in current camera viewport

    _batch_xyz.clear();
    _batch_uv.clear();

    std::size_t tiles =0;
    // iterating from left-lower corner of the room to the right-upper (spelunky-ds convention)
    for (int x = 0; x < Consts::MAP_GAME_WIDTH_TILES; x++) {
        for (int y = 0; y < Consts::MAP_GAME_HEIGHT_TILES; y++) {
            MapTile *t = level.getLevel().map_tiles[x][y];
//            if (!t->in_viewport(&camera)) continue;
            int tile_type = t->mapTileType;
            float coordinates[4][2];

            if (tile_type > 0) {
                set_texture_pointer_to_tile(tile_type, coordinates);
            } else {
                // place a piece of background
                // FIXME Find correct order of placing background tiles and put it into this matrix
                int bg_matrix[8][2] = {
                        {15, 16},
                        {13, 14},
                        {11, 12},
                        {9,  10},
                        {7,  8},
                        {5,  6},
                        {3,  4},
                        {1,  2},
                };

                int bgr_type = bg_matrix[y % 8][x % 2] + 42;
                set_texture_pointer_to_tile(bgr_type, coordinates);
            }


            // left lower  0
            // left upper  1
            // right upper 2
            // right lower 3

            _batch_uv.push_back(coordinates[0][0]);
            _batch_uv.push_back(coordinates[0][1]);
            _batch_xyz.push_back(0 + x);
            _batch_xyz.push_back(0 + y);
            _batch_xyz.push_back(0);

            _batch_uv.push_back(coordinates[1][0]);
            _batch_uv.push_back(coordinates[1][1]);
            _batch_xyz.push_back(0 + x);
            _batch_xyz.push_back(1 + y);
            _batch_xyz.push_back(0);

            _batch_uv.push_back(coordinates[2][0]);
            _batch_uv.push_back(coordinates[2][1]);
            _batch_xyz.push_back(1 + x);
            _batch_xyz.push_back(1 + y);
            _batch_xyz.push_back(0);

            _batch_uv.push_back(coordinates[2][0]);
            _batch_uv.push_back(coordinates[2][1]);
            _batch_xyz.push_back(1 + x);
            _batch_xyz.push_back(1 + y);
            _batch_xyz.push_back(0);

            _batch_uv.push_back(coordinates[3][0]);
            _batch_uv.push_back(coordinates[3][1]);
            _batch_xyz.push_back(1 + x);
            _batch_xyz.push_back(0 + y);
            _batch_xyz.push_back(0);

            _batch_uv.push_back(coordinates[0][0]);
            _batch_uv.push_back(coordinates[0][1]);
            _batch_xyz.push_back(0 + x);
            _batch_xyz.push_back(0 + y);
            _batch_xyz.push_back(0);

            tiles++;
        }
    }

    DebugGlCall(glEnableClientState(GL_VERTEX_ARRAY));
    DebugGlCall(glEnableClientState(GL_TEXTURE_COORD_ARRAY));

    DebugGlCall(glMatrixMode(GL_MODELVIEW));
    DebugGlCall(glLoadIdentity());
    DebugGlCall(glTranslatef(0, 0, 0));
    DebugGlCall(graphics_utils::look_at(camera));

    DebugGlCall(glVertexPointer(3, GL_FLOAT, 0, _batch_xyz.data()));
    DebugGlCall(glTexCoordPointer(2, GL_FLOAT, 0, _batch_uv.data()));

    DebugGlCall(glDrawArrays(GL_TRIANGLES, 0, 6 * tiles));
}

namespace
{
    void set_texture_pointer_to_tile(int type, float coordinates[4][2])
    {

        type--;

        int rows = 512 / 16;
        int columns = 32 / 16;

        // since coordinates must be normalized (between 0 and 1)
        float x_unit = 1.0f / columns;
        float y_unit = 1.0f / rows;

        float x_offset = 0;
        if (type % 2 == 1) x_offset++;
        float y_offset = std::floor((float) type / 2);

        // now it stores left-upper corner
        x_offset *= x_unit;
        y_offset *= y_unit;

        // left lower  0
        // left upper  1
        // right upper 2
        // right lower 3

        float onePixelX = 1.0f / 32;
        float onePixelY = 1.0f / 512;

        coordinates[1][0] = x_offset + onePixelX;
        coordinates[1][1] = y_offset + y_unit - onePixelY;

        coordinates[0][0] = x_offset + onePixelX;
        coordinates[0][1] = y_offset + onePixelY;

        coordinates[3][0] = x_offset + x_unit - onePixelX;
        coordinates[3][1] = y_offset + onePixelY;

        coordinates[2][0] = x_offset + x_unit - onePixelX;
        coordinates[2][1] = y_offset + y_unit - onePixelY;
    }
}
