#pragma once

#include "benchmarking/Scene.hpp"
#include "Level.hpp"
#include "TileBatch.hpp"
#include "MapTile.hpp"
#include "EntityRegistry.hpp"
#include "prefabs/effects/Explosion.hpp"
#include "other/ParticleGenerator.hpp"
#include "prefabs/props/QuitSign.hpp"
#include "prefabs/props/MainLogo.hpp"

class PropsScene : public Scene
{
public:
    virtual void update(const FrameStats& frame_stats,
                        ModelViewCamera& model_view_camera,
                        ScreenSpaceCamera& screen_space_camera) override
    {
    }

    void spawn_entities() override
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& tile_batch = Level::instance().get_tile_batch();

        for (int x = 0; x < Consts::LEVEL_WIDTH_TILES; x++)
        {
            for (int y = 0; y < Consts::LEVEL_HEIGHT_TILES; y++)
            {
                tile_batch.map_tiles[x][y]->match_tile(MapTileType::NOTHING);
                tile_batch.map_tiles[x][y]->destroyable = false;
                prefabs::QuitSign::create(x, y);
            }
        }

        tile_batch.generate_frame();
        tile_batch.generate_cave_background();
        tile_batch.batch_vertices();
        tile_batch.add_render_entity(registry);
    }

    const char* scene_title() const override
    {
        return "Props (benchmarking)";
    }
};
