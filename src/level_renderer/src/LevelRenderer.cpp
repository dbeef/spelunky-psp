#include "LevelRenderer.hpp"
#include "LevelGenerator.hpp"
#include "video/Context.hpp"
#include "glad/glad.h"
#include "logger/log.h"
#include "cJSON.h"

#include "graphics_utils/LookAt.hpp"
#include "graphics_utils/CreateTexture.hpp"
#include "graphics_utils/DebugGlCall.hpp"

#include <string>
#include <cstring>

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

    try
    {
        char* json_ptr = &level_tiles_json::data[0];
        cJSON* document_root = cJSON_Parse(json_ptr);
        if (!document_root)
        {
            const char *error_ptr = cJSON_GetErrorPtr();
            if (error_ptr != nullptr)
            {
                throw std::runtime_error(std::string(error_ptr));
            }
            else
            {
                throw std::runtime_error("Failed to parse document root.");
            }
        }

        std::string filename;
        cJSON* filename_document = cJSON_GetObjectItemCaseSensitive(document_root, "image");
        assert(cJSON_IsString(filename_document) && (filename_document->valuestring != nullptr));
        filename = filename_document->valuestring;

        uint16_t image_width = 0;
        cJSON* image_width_document = cJSON_GetObjectItemCaseSensitive(document_root, "image_width");
        assert(cJSON_IsNumber(image_width_document));
        image_width = image_width_document->valueint;

        uint16_t image_height = 0;
        cJSON* image_height_document = cJSON_GetObjectItemCaseSensitive(document_root, "image_height");
        assert(cJSON_IsNumber(image_height_document));
        image_height = image_height_document->valueint;

        log_info("Parsing metadata out of texture atlas: %s, width: %i, height: %i",
                filename.c_str(), image_width, image_height);

        for (std::size_t index = 0; index < static_cast<std::uint32_t>(MapTileType::_SIZE); index++)
        {
            _tiles[index] = RenderTile::fromJson(static_cast<MapTileType>(index), document_root);
            _tiles[index].normalize(image_width, image_height);
        }

        cJSON_Delete(document_root);
    }
    catch (const std::exception& e)
    {
        log_error("Exception while parsing spritesheet metadata JSON: %s", e.what());
        assert(false);
    }

    log_info("Done loading tilesheet.");
}

void LevelRenderer::render() const
{
    // Interleaving vertex attributes instead of separate buffers for small performance boost from data locality:
    const auto* vertices = reinterpret_cast<const char*>(_render_batch.merged.data());
    const auto* uvs = vertices + 2 * sizeof(GLshort);

    const size_t stride = 2 * sizeof(GLshort) + 2 * sizeof(GLfloat);

    DebugGlCall(glBindTexture(GL_TEXTURE_2D, _tilesheet));

    DebugGlCall(glVertexPointer(2, GL_SHORT, stride, vertices));
    DebugGlCall(glTexCoordPointer(2, GL_FLOAT, stride, uvs));

    DebugGlCall(glDrawElements(GL_TRIANGLES, _render_batch.indices.size(), GL_UNSIGNED_SHORT, _render_batch.indices.data()));
}

bool LevelRenderer::re_batching_needed()
{
    // Re-batch vertices only when camera view is out of already batched vertices:
    const auto& camera = Camera::instance();

    const auto camera_x_in_tiles = static_cast<int32_t >(camera.getX());
    const auto camera_y_in_tiles = static_cast<int32_t >(camera.getY());

    bool rebatching_needed = camera_x_in_tiles != _last_camera_x_in_tiles ||
                             camera_y_in_tiles != _last_camera_y_in_tiles;

    _last_camera_x_in_tiles = camera_x_in_tiles;
    _last_camera_y_in_tiles = camera_y_in_tiles;

    return rebatching_needed;
}

void LevelRenderer::batch_vertices()
{
    auto& level = LevelGenerator::instance();
    const auto& camera = Camera::instance();

    _render_batch.xyz.clear();
    _render_batch.uv.clear();
    _render_batch.indices.clear();
    _render_batch.tile_counter = 0;

    // FIXME: Rewrite level generator for more sane convention of storing tiles.
    // iterating from left-lower corner of the room to the right-upper (spelunky-ds convention)
    for (int x = 0; x < Consts::MAP_GAME_WIDTH_TILES; x++) {
        for (int y = 0; y < Consts::MAP_GAME_HEIGHT_TILES; y++) {

            MapTile *t = level.getLevel().map_tiles[x][y];

            // FIXME: Remove x/y fields from MapTile, as they are redundant.
            assert(level.getLevel().map_tiles[x][y]->x == x);
            assert(level.getLevel().map_tiles[x][y]->y == y);

            if (!t->in_viewport(&camera)) continue;

            auto tile_type = static_cast<int>(t->mapTileType);
            auto& tile = _tiles[tile_type];

            tile.push_positions(_render_batch.xyz, x, y);
            tile.push_indices(_render_batch.indices, _render_batch.tile_counter);
            tile.push_uvs(_render_batch.uv);
            _render_batch.tile_counter++;
        }
    }

    // This could be done in the loop before, but this way is more readable, and does not affect performance much
    // as batching vertices is not done very often.
    assert(_render_batch.xyz.size() == _render_batch.uv.size());
    for (std::size_t index = 0; index < _render_batch.xyz.size(); index += 2)
    {
        Vertex vertex{};
        vertex.x = _render_batch.xyz[index];
        vertex.y = _render_batch.xyz[index + 1];
        vertex.u = _render_batch.uv[index];
        vertex.v = _render_batch.uv[index + 1];
        _render_batch.merged.push_back(vertex);
    }
}
