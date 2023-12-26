#include "game-loop/GameLoopMainMenuState.hpp"
#include "game-loop/GameLoop.hpp"

#include "logger/log.h"
#include "ModelViewCamera.hpp"
#include "ScreenSpaceCamera.hpp"
#include "Level.hpp"
#include "audio/Audio.hpp"
#include "EntityRegistry.hpp"
#include "other/Inventory.hpp"

#include "system/RenderingSystem.hpp"
#include "system/ScriptingSystem.hpp"
#include "system/PhysicsSystem.hpp"
#include "system/AnimationSystem.hpp"
#include "system/InputSystem.hpp"
#include "system/DisposingSystem.hpp"
#include "system/ParticleSystem.hpp"
#include "system/ItemSystem.hpp"

#include "components/specialized/LevelSummaryTracker.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/specialized/PauseOverlayComponent.hpp"
#include "components/specialized/MainDudeComponent.hpp"

#include "prefabs/props/StartSign.hpp"
#include "prefabs/props/GithubLink.hpp"
#include "prefabs/props/ScoresSign.hpp"
#include "prefabs/props/TutorialSign.hpp"
#include "prefabs/props/QuitSign.hpp"
#include "prefabs/props/MainLogo.hpp"
#include "prefabs/props/CopyrightsSign.hpp"
#include "prefabs/items/RopeChainElement.hpp"
#include "prefabs/items/Flare.hpp"
#include "prefabs/main-dude/MainDude.hpp"
#include "prefabs/ui/PauseOverlay.hpp"
#include "prefabs/ui/CheatConsole.hpp"

#include <cmath>

namespace
{
    const Point2D PLAY_COORDS = {5, 9};
    const Point2D SCORES_COORDS = {9, 9};
    const Point2D TUTORIAL_COORDS = {1, 9};
    const float EXIT_LEVEL_Y = 1.0f;
}

GameLoopBaseState *GameLoopMainMenuState::update(GameLoop& game_loop, uint32_t delta_time_ms)
{
    auto& registry = EntityRegistry::instance().get_registry();

    auto& rendering_system = game_loop._rendering_system;
    auto& scripting_system = game_loop._scripting_system;
    auto& physics_system = game_loop._physics_system;
    auto& animation_system = game_loop._animation_system;
    auto& input_system = game_loop._input_system;
    auto& disposing_system = game_loop._disposing_system;
    auto& particle_system = game_loop._particle_system;
    auto& item_system = game_loop._item_system;

    rendering_system->update(delta_time_ms);
    input_system->update(delta_time_ms);

    auto& pause = registry.get<PauseOverlayComponent>(_pause_overlay);

    if (pause.is_paused())
    {
        if (pause.is_quit_requested())
        {
            log_info("Quit requested.");
            game_loop._exit = true;
        }

        pause.update(registry);
    }
    else
    {
        physics_system->update(delta_time_ms);
        animation_system->update(delta_time_ms);
        scripting_system->update(delta_time_ms);
        disposing_system->update(delta_time_ms);
        particle_system->update(delta_time_ms);
        item_system->update(delta_time_ms);
    }

    auto& dude = registry.get<MainDudeComponent>(_main_dude);
    auto& position = registry.get<PositionComponent>(_main_dude);

    if (dude.entered_door())
    {
        const Point2D position_in_tiles = {std::floor(position.x_center), std::floor(position.y_center)};

        if (position_in_tiles == PLAY_COORDS)
        {
            return &game_loop._states.playing;
        }
        else if (position_in_tiles == SCORES_COORDS)
        {
            return &game_loop._states.scores;
        }
        else
        {
            assert(false);
        }
    }

    if (position.y_center <= EXIT_LEVEL_Y)
    {
        log_info("Quitting using rope.");
        game_loop._exit = true;
    }

    auto& cheat_console = registry.get<prefabs::CheatConsoleComponent>(_cheat_console);
    if (cheat_console.is_state_change_requested()) {
        return game_loop.get_game_loop_state_ptr(cheat_console.get_requested_state());
    }

    return this;
}

void GameLoopMainMenuState::enter(GameLoop& game_loop)
{
    log_info("Entered GameLoopMainMenuState");

    Inventory::instance().set_starting_inventory();

    auto& registry = EntityRegistry::instance().get_registry();
    auto& rendering_system = game_loop._rendering_system;
    
    Audio::instance().play(MusicType::TITLE);

    Level::instance().get_tile_batch().generate_frame();
    Level::instance().get_tile_batch().initialise_tiles_from_splash_screen(SplashScreenType::MAIN_MENU);
    Level::instance().get_tile_batch().generate_cave_background();
    Level::instance().get_tile_batch().batch_vertices();
    Level::instance().get_tile_batch().add_render_entity(registry);

    // Splash screens are copied into the [0, 0] position (left-upper corner), center on them:
    auto& model_view_camera = rendering_system->get_model_view_camera();
    model_view_camera.set_x_not_rounded(game_loop._viewport->get_width_world_units() / 4.0f);
    model_view_camera.set_y_not_rounded(game_loop._viewport->get_height_world_units() / 4.0f);
    model_view_camera.update_gl_modelview_matrix();

    prefabs::StartSign::create(5.55, 9.0);
    prefabs::ScoresSign::create(9.55, 9.0);
    prefabs::TutorialSign::create(1.0, 8.5);
    prefabs::QuitSign::create(16.0, 1.5);
    prefabs::MainLogo::create(9.75, 5.5);
    prefabs::CopyrightsSign::create(10.0, 10.75);
    prefabs::GithubLink::create(9.0, 1.0);
    prefabs::Flare::create(16.5, 9.5);

    for (int index = 0; index < 18; index++)
    {
        const float x = 17.5f;
        const float y = 0.5f * index;
        prefabs::RopeChainElement::create(x, y);
        auto& tile_batch = Level::instance().get_tile_batch();
        tile_batch.map_tiles[static_cast<int>(x)][static_cast<int>(y)]->climbable = true;
    }

    _pause_overlay = prefabs::PauseOverlay::create(game_loop._viewport, PauseOverlayComponent::Type::MAIN_MENU);
    _main_dude = prefabs::MainDude::create(17.5, 9.5);
    _cheat_console = prefabs::CheatConsole::create(game_loop._viewport);

    game_loop._level_summary_tracker->reset();

    rendering_system->sort();
}

void GameLoopMainMenuState::exit(GameLoop& game_loop)
{
    auto& registry = EntityRegistry::instance().get_registry();
    Audio::instance().stop();

    auto& dude = registry.get<MainDudeComponent>(_main_dude);
    auto& dude_item_carrier_component = registry.get<ItemCarrierComponent>(_main_dude);

    // Save collected item types in inventory which is persistent between the levels:
    auto& inventory = Inventory::instance();
    inventory.set_items(dude_item_carrier_component.get_items());

    registry.clear();
}
