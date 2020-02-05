#include "LevelRenderer.hpp"
#include "LevelGenerator.hpp"
#include "video/Context.hpp"

#include "glad/glad.h"
#include "logger/log.h"

#include "graphics_utils/LookAt.hpp"
#include "graphics_utils/CreateTexture.hpp"
#include "graphics_utils/DebugGlCall.hpp"

#include "tao/json.hpp"

#include <string>

namespace
{
    namespace level_tiles_json
    {
        #include "generated/level_tiles.json.hpp"
    }

    namespace level_tiles_image
    {
        #include "generated/level_tiles.png.hpp"
    }
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

    _tilesheet = graphics_utils::createTexture(level_tiles_image::data, sizeof(level_tiles_image::data));
    assert(_tilesheet);

    char* json_ptr = &level_tiles_json::data[0];
    auto document_root = tao::json::from_string(json_ptr);

    try
    {
        std::string filename = document_root["image"].get_string();
        uint16_t image_width = document_root["image_width"].get_unsigned();
        uint16_t image_height = document_root["image_height"].get_unsigned();

        log_info("Parsing metadata out of texture atlas: %s, width: %i, height: %i",
                filename.c_str(), image_width, image_height);

        for (std::size_t index = 0; index < static_cast<std::uint32_t>(MapTileType::_SIZE); index++)
        {
            _tiles[index] = RenderTile::fromJson(static_cast<MapTileType>(index), document_root);
            _tiles[index].normalize(image_width, image_height);
        }
    }
    catch (const std::exception& e)
    {
        log_error("Exception while parsing spritesheet metadata JSON: %s", e.what());
        assert(false);
    }

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
    _batch_indices.clear();
    _tile_counter = 0;

    std::size_t tiles = 0;
    // iterating from left-lower corner of the room to the right-upper (spelunky-ds convention)
    for (int x = 0; x < Consts::MAP_GAME_WIDTH_TILES; x++) {
        for (int y = 0; y < Consts::MAP_GAME_HEIGHT_TILES; y++) {

            MapTile *t = level.getLevel().map_tiles[x][y];
            auto tile_type = static_cast<int>(t->mapTileType);
            auto& tile = _tiles[tile_type];

            tile.push_positions(_batch_xyz, x, y);
            tile.push_indices(_batch_indices, _tile_counter);
            tile.push_uvs(_batch_uv);
            _tile_counter++;

            tiles++;
        }
    }

    DebugGlCall(glMatrixMode(GL_MODELVIEW));
    DebugGlCall(glLoadIdentity());
    DebugGlCall(glTranslatef(0, 0, 0));
    DebugGlCall(graphics_utils::look_at(camera));

    DebugGlCall(glEnableClientState(GL_VERTEX_ARRAY));
    DebugGlCall(glEnableClientState(GL_TEXTURE_COORD_ARRAY));

    DebugGlCall(glVertexPointer(2, GL_FLOAT, 0, _batch_xyz.data()));
    DebugGlCall(glTexCoordPointer(2, GL_FLOAT, 0, _batch_uv.data()));

    DebugGlCall(glDrawElements(GL_TRIANGLES, _batch_indices.size(), GL_UNSIGNED_INT, _batch_indices.data()));
}
