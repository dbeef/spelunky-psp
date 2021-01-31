#include "prefabs/ui/DeathOverlay.hpp"
#include "prefabs/ui/HudOverlay.hpp"
#include "prefabs/main-dude/MainDude.hpp"
#include "prefabs/ui/PauseOverlay.hpp"

#include "game-loop/GameLoopPlayingState.hpp"
#include "game-loop/GameLoop.hpp"

#include "EntityRegistry.hpp"

#include "components/specialized/HudOverlayComponent.hpp"
#include "components/specialized/PauseOverlayComponent.hpp"
#include "components/specialized/DeathOverlayComponent.hpp"
#include "components/specialized/LevelSummaryTracker.hpp"
#include "components/specialized/MainDudeComponent.hpp"

#include "system/RenderingSystem.hpp"
#include "system/ItemSystem.hpp"
#include "system/CollectibleSystem.hpp"
#include "system/ScriptingSystem.hpp"
#include "system/PhysicsSystem.hpp"
#include "system/AnimationSystem.hpp"
#include "system/InputSystem.hpp"
#include "system/DisposingSystem.hpp"
#include "system/ParticleSystem.hpp"

#include "logger/log.h"
#include "ModelViewCamera.hpp"
#include "ScreenSpaceCamera.hpp"
#include "CameraType.hpp"
#include "Level.hpp"
#include "audio/Audio.hpp"
#include "populator/Populator.hpp"

GameLoopBaseState *GameLoopPlayingState::update(GameLoop& game_loop, uint32_t delta_time_ms)
{
    auto& registry = EntityRegistry::instance().get_registry();

    auto& rendering_system = game_loop._rendering_system;
    auto& scripting_system = game_loop._scripting_system;
    auto& physics_system = game_loop._physics_system;
    auto& animation_system = game_loop._animation_system;
    auto& collectible_system = game_loop._collectible_system;
    auto& input_system = game_loop._input_system;
    auto& item_system = game_loop._item_system;
    auto& disposing_system = game_loop._disposing_system;
    auto& particle_system = game_loop._particle_system;

    // Adjust camera to follow main dude:
    auto& position = registry.get<PositionComponent>(_main_dude);
    auto& model_view_camera = game_loop._rendering_system->get_model_view_camera();

    model_view_camera.adjust_to_bounding_box(position.x_center, position.y_center);
    model_view_camera.adjust_to_level_boundaries(Consts::LEVEL_WIDTH_TILES, Consts::LEVEL_HEIGHT_TILES);
    model_view_camera.update_gl_modelview_matrix();

    rendering_system->update(delta_time_ms);
    input_system->update(delta_time_ms);

    auto& pause = registry.get<PauseOverlayComponent>(_pause_overlay);

    if (pause.is_paused())
    {
        if (pause.is_death_requested())
        {
            log_info("Death requested.");
            auto& dude = registry.get<MainDudeComponent>(_main_dude);
            dude.enter_dead_state();
        }

        if (pause.is_quit_requested())
        {
            log_info("Quit requested.");
            game_loop._exit = true;
        }

        pause.update(registry);
    }
    else
    {
        scripting_system->update(delta_time_ms);
        physics_system->update(delta_time_ms);
        animation_system->update(delta_time_ms);
        item_system->update(delta_time_ms);
        disposing_system->update(delta_time_ms);
        collectible_system->update(delta_time_ms);
        particle_system->update(delta_time_ms);
    }

    auto& dude = registry.get<MainDudeComponent>(_main_dude);

    if (dude.entered_door())
    {
        return &game_loop._states.level_summary;
    }

    auto& death = registry.get<DeathOverlayComponent>(_death_overlay);

    if (death.is_scores_requested())
    {
        return &game_loop._states.scores;
    }

    game_loop._level_summary_tracker->update(delta_time_ms);

    return this;
}

void GameLoopPlayingState::enter(GameLoop& game_loop)
{
    log_info("Entered GameLoopPlayingState");

    auto& registry = EntityRegistry::instance().get_registry();

    Audio::instance().play(MusicType::CAVE);

    Level::instance().get_tile_batch().generate_new_level_layout();
    Level::instance().get_tile_batch().initialise_tiles_from_room_layout();
    Level::instance().get_tile_batch().generate_frame();
    Level::instance().get_tile_batch().generate_cave_background();
    Level::instance().get_tile_batch().batch_vertices();
    Level::instance().get_tile_batch().add_render_entity(registry);

    // Update main dude:
    MapTile *entrance = nullptr;
    Level::instance().get_tile_batch().get_first_tile_of_given_type(MapTileType::ENTRANCE, entrance);
    assert(entrance);

    float pos_x = entrance->x + (MapTile::PHYSICAL_WIDTH / 2.0f);
    float pos_y = entrance->y + (MapTile::PHYSICAL_HEIGHT / 2.0f);

    // Update main dude:
    _main_dude = prefabs::MainDude::create(pos_x, pos_y);
    _pause_overlay = prefabs::PauseOverlay::create(game_loop._viewport, PauseOverlayComponent::Type::PLAYING);
    _death_overlay = prefabs::DeathOverlay::create(game_loop._viewport);
    _hud = prefabs::HudOverlay::create(game_loop._viewport);

    game_loop._rendering_system->sort();

    auto& dude = registry.get<MainDudeComponent>(_main_dude);
    dude.add_observer(this);

    auto& death = registry.get<DeathOverlayComponent>(_death_overlay);
    death.disable_input();

    populator::generate_loot(game_loop._level_summary_tracker);
    populator::generate_npc(game_loop._level_summary_tracker);
    game_loop._level_summary_tracker->entered_new_level();
}

void GameLoopPlayingState::exit(GameLoop& game_loop)
{
    auto& registry = EntityRegistry::instance().get_registry();
    Audio::instance().stop();

    auto& dude = registry.get<MainDudeComponent>(_main_dude);
    dude.remove_observer(this);

    registry.clear();
}

void GameLoopPlayingState::on_notify(const MainDudeEvent* event)
{
    auto& registry = EntityRegistry::instance().get_registry();

    switch(*event)
    {
        case MainDudeEvent::DIED:
        {
            // TODO: Maybe just remove components when they are no longer needed?
            auto& death = registry.get<DeathOverlayComponent>(_death_overlay);
            auto& pause = registry.get<PauseOverlayComponent>(_pause_overlay);

            death.launch();
            death.enable_input();

            pause.unpause();
            pause.hide(registry);
            pause.disable_input();

            registry.destroy(_hud);
            _hud = entt::null;

            break;
        }
        default: break;
    }
}
