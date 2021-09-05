#include "benchmarking/PropsScene.hpp"
#include "TileBatch.hpp"
#include "Level.hpp"
#include "EntityRegistry.hpp"
#include "prefabs/props/QuitSign.hpp"

void PropsScene::update(const FrameStats &frame_stats,
                        ModelViewCamera &model_view_camera,
                        ScreenSpaceCamera &screen_space_camera)
{
}

const char *PropsScene::scene_title() const
{
    return "Props";
}

void PropsScene::spawn_entities()
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& tile_batch = Level::instance().get_tile_batch();

    tile_batch.clear();

    for (int x = 0; x < Consts::LEVEL_WIDTH_TILES; x++)
    {
        for (int y = 0; y < Consts::LEVEL_HEIGHT_TILES; y++)
        {
            prefabs::QuitSign::create(x, y);
        }
    }

    tile_batch.generate_frame();
    tile_batch.generate_cave_background();
    tile_batch.batch_vertices();
    tile_batch.add_render_entity(registry);
}
