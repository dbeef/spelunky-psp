#include "game-loop/GameLoopPlayingState.hpp"
#include "game-loop/GameLoop.hpp"

#include "game-entities/GameEntity.hpp"
#include "game-entities/HUD.hpp"
#include "game-entities/PauseOverlay.hpp"
#include "game-entities/DeathOverlay.hpp"
#include "game-entities/LevelSummaryTracker.hpp"
#include "system/GameEntitySystem.hpp"
#include "main-dude/MainDude.hpp"

#include "logger/log.h"
#include "ModelViewCamera.hpp"
#include "ScreenSpaceCamera.hpp"
#include "Renderer.hpp"
#include "Level.hpp"
#include "audio/Audio.hpp"
#include "populator/Populator.hpp"
#include "other/World.hpp"
#include "TileBatch.hpp"

GameLoopBaseState *GameLoopPlayingState::update(GameLoop& game_loop, uint32_t delta_time_ms)
{
    auto& model_view_camera = game_loop._cameras.model_view;
    auto& screen_space_camera = game_loop._cameras.screen_space;
    auto& main_dude = game_loop._world->get_main_dude();
    auto& renderer = Renderer::instance();

    // Adjust camera to follow main dude:

    auto dude_physics = main_dude->get_physics_component();
    assert(dude_physics);

    auto x = dude_physics->get_x_position();
    auto y = dude_physics->get_y_position();

    model_view_camera.adjust_to_bounding_box(x, y);
    model_view_camera.adjust_to_level_boundaries(Consts::LEVEL_WIDTH_TILES, Consts::LEVEL_HEIGHT_TILES);

    // Remove render entities marked for disposal:

    renderer.update();

    // Render:

    model_view_camera.update_gl_modelview_matrix();
    model_view_camera.update_gl_projection_matrix();

    renderer.render(Renderer::EntityType::MODEL_VIEW_SPACE);

    screen_space_camera.update_gl_modelview_matrix();
    screen_space_camera.update_gl_projection_matrix();

    renderer.render(Renderer::EntityType::SCREEN_SPACE);

    // Update game entities:

    if (_pause_overlay->is_paused())
    {
        if (_pause_overlay->is_death_requested())
        {
            log_info("Death requested.");
            main_dude->enter_dead_state();
        }

        if (_pause_overlay->is_quit_requested())
        {
            log_info("Quit requested.");
            game_loop._exit = true;
        }

        _pause_overlay->update(game_loop._world.get(), delta_time_ms);
    }
    else
    {
        game_loop._game_entity_system->update(game_loop._world.get(),  delta_time_ms);
    }

    // Other:

    if (main_dude->entered_door())
    {
        return &game_loop._states.level_summary;
    }

    if (_death_overlay->is_scores_requested())
    {
        return &game_loop._states.scores;
    }

    return this;
}

void GameLoopPlayingState::enter(GameLoop& game_loop)
{
    log_info("Entered GameLoopPlayingState");

    Audio::instance().play(MusicType::CAVE);

    auto& tile_batch = game_loop._world->get_tile_batch();

    tile_batch->generate_new_level_layout();
    tile_batch->initialise_tiles_from_room_layout();
    tile_batch->generate_frame();
    tile_batch->generate_cave_background();
    tile_batch->batch_vertices();

    // Update main dude:
    auto& main_dude = game_loop._world->get_main_dude();
    auto main_dude_physics = main_dude->get_physics_component();
    assert(main_dude_physics);

    game_loop._world->get_main_dude()->enter_standing_state();
    game_loop._game_entity_system->add(main_dude);

    main_dude_physics->set_velocity(0, 0);

    MapTile *entrance = nullptr;
    tile_batch->get_first_tile_of_given_type(MapTileType::ENTRANCE, entrance);
    assert(entrance);
    main_dude_physics->set_position(entrance->get_center());

    // Subscribe on main dude's events:
    main_dude->add_observer(this);

    // Create HUD:
    _hud = std::make_shared<HUD>(game_loop._viewport);
    game_loop._game_entity_system->add(_hud);

    // Create pause overlay:
    _pause_overlay = std::make_shared<PauseOverlay>(game_loop._viewport, PauseOverlay::Type::PLAYING);
    game_loop._game_entity_system->add(_pause_overlay);
    
    // Create death overlay:
    _death_overlay = std::make_shared<DeathOverlay>(game_loop._viewport);
    _death_overlay->disable_input();
    game_loop._game_entity_system->add(_death_overlay);

    // Generate loot:
    auto loot_entities = populator::generate_loot(game_loop._level_summary_tracker, game_loop._world.get());
    game_loop._game_entity_system->add(loot_entities);

    // Make main dude appear on the foreground:
    Renderer::instance().sort_by_layer();

    game_loop._level_summary_tracker->entered_new_level();
    game_loop._game_entity_system->add(game_loop._level_summary_tracker);
}

void GameLoopPlayingState::exit(GameLoop& game_loop)
{
    Audio::instance().stop();

    _death_overlay = nullptr;
    _pause_overlay = nullptr;
    _hud = nullptr;

    auto& main_dude = game_loop._world->get_main_dude();
    main_dude->remove_observer(this);
    game_loop._game_entity_system->remove_all();
}

void GameLoopPlayingState::on_notify(const MainDudeEvent* event)
{
    switch(*event)
    {
        case MainDudeEvent::DIED:
        {
            _death_overlay->launch();
            _death_overlay->enable_input();
            _pause_overlay->reset();
            _pause_overlay->disable_input();
            _hud->hide();
            break;
        }
        default: break;
    }
}
