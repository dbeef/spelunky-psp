#include "benchmarking/FlaresScene.hpp"
#include "Level.hpp"
#include "TileBatch.hpp"
#include "MapTile.hpp"
#include "EntityRegistry.hpp"
#include "other/ParticleGenerator.hpp"
#include "video/Video.hpp"

void FlaresScene::update(const FrameStats &frame_stats,
                         ModelViewCamera &model_view_camera,
                         ScreenSpaceCamera &screen_space_camera)
{
    _camera_move_timer_ms += frame_stats.total_delta_time_ms;

    if (_camera_move_timer_ms > 20)
    {
        _camera_move_timer_ms = 0;
        model_view_camera.set_x(model_view_camera.get_x() + (MapTile::PHYSICAL_WIDTH / 100.f));
        model_view_camera.set_y(model_view_camera.get_y() + (MapTile::PHYSICAL_HEIGHT / 100.f));
        model_view_camera.adjust_to_level_boundaries(Consts::LEVEL_WIDTH_TILES, Consts::LEVEL_HEIGHT_TILES);
        model_view_camera.update_gl_modelview_matrix();
    }

    _flares_spawn_timer_ms += frame_stats.total_delta_time_ms;

    if (_flares_spawn_timer_ms > 15)
    {
        _flares_spawn_timer_ms = 0;

        const float camera_x = model_view_camera.get_x_screen_center();
        const float camera_y = model_view_camera.get_y_screen_center();

        ParticleGenerator().particle_type(ParticleType::FLAME)
                .position(camera_x, camera_y)
                .max_velocity(0.25f, 0.25f)
                .quantity(2)
                .finalize();
    }
}

void FlaresScene::spawn_entities()
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& tile_batch = Level::instance().get_tile_batch();

    tile_batch.clear();
    tile_batch.generate_frame();
    tile_batch.generate_cave_background();
    tile_batch.batch_vertices();
    tile_batch.add_render_entity(registry);
}

const char* FlaresScene::scene_title() const
{
    return "Flares";
}
