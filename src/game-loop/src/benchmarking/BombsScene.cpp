#include "benchmarking/BombsScene.hpp"
#include "Level.hpp"
#include "TileBatch.hpp"
#include "MapTile.hpp"
#include "EntityRegistry.hpp"
#include "other/ParticleGenerator.hpp"
#include "video/Video.hpp"
#include "prefabs/items/Bomb.hpp"
#include "prefabs/items/Rock.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/ActivableComponent.hpp"

namespace
{
    const float _level_center_x = Consts::LEVEL_WIDTH_TILES / 2.f;
    const float _level_center_y = Consts::LEVEL_WIDTH_TILES / 2.f;
}

void BombsScene::update(const FrameStats &frame_stats,
                         ModelViewCamera &model_view_camera,
                         ScreenSpaceCamera &screen_space_camera)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& rock_position = registry.get<PositionComponent>(_rock);

    model_view_camera.adjust_to_bounding_box(rock_position.x_center, rock_position.y_center);
    model_view_camera.adjust_to_level_boundaries(Consts::LEVEL_WIDTH_TILES, Consts::LEVEL_HEIGHT_TILES);
    model_view_camera.update_gl_modelview_matrix();

    _bombs_spawn_timer_ms += frame_stats.total_delta_time_ms;

    if (_bombs_spawn_timer_ms > 100)
    {
        _bombs_spawn_timer_ms = 0;
        auto bomb_entity = prefabs::Bomb::create(rock_position.x_center, rock_position.y_center);
        auto& activable = registry.get<ActivableComponent>(bomb_entity);
        activable.activated = true;
    }
}

void BombsScene::spawn_entities()
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& tile_batch = Level::instance().get_tile_batch();

    tile_batch.clear();
    tile_batch.generate_frame();
    tile_batch.generate_cave_background();

    for (int x = 1; x < Consts::LEVEL_WIDTH_TILES - 1; x++)
    {
        for (int y = 1; y < Consts::LEVEL_HEIGHT_TILES - 1; y++)
        {
            tile_batch.map_tiles[x][y]->match_tile(MapTileType::STONE_BLOCK);
        }
    }

    tile_batch.batch_vertices();
    tile_batch.add_render_entity(registry);

    _rock = prefabs::Rock::create(_level_center_x, _level_center_y);
}

const char* BombsScene::scene_title() const
{
    return "Bombs";
}
