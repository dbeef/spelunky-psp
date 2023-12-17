#include "game-loop/GameLoopSandboxState.hpp"
#include "game-loop/GameLoop.hpp"

#include "EntityRegistry.hpp"

#include "prefabs/main-dude/MainDude.hpp"
#include "prefabs/ui/ScoresOverlay.hpp"
#include "prefabs/ui/PauseOverlay.hpp"
#include "prefabs/props/ResetSign.hpp"

#include "components/specialized/PauseOverlayComponent.hpp"
#include "components/specialized/MainDudeComponent.hpp"

#include "system/RenderingSystem.hpp"
#include "system/ScriptingSystem.hpp"
#include "system/PhysicsSystem.hpp"
#include "system/ShoppingSystem.hpp"
#include "system/AnimationSystem.hpp"
#include "system/InputSystem.hpp"
#include "system/DisposingSystem.hpp"
#include "system/ParticleSystem.hpp"
#include "system/ItemSystem.hpp"

#include "populator/Populator.hpp"
#include "logger/log.h"
#include "ModelViewCamera.hpp"
#include "ScreenSpaceCamera.hpp"
#include "CameraType.hpp"
#include "Level.hpp"
#include "other/Inventory.hpp"
#include "prefabs/ui/CheatConsole.hpp"

GameLoopBaseState *GameLoopSandboxState::update(GameLoop& game_loop, uint32_t delta_time_ms)
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

    // Adjust camera to follow main dude:
    auto& position = registry.get<PositionComponent>(_main_dude);
    auto& model_view_camera = game_loop._rendering_system->get_model_view_camera();

    model_view_camera.adjust_to_bounding_box(position.x_center, position.y_center);
    model_view_camera.adjust_to_level_boundaries(Consts ::LEVEL_WIDTH_TILES, Consts::LEVEL_HEIGHT_TILES);
    model_view_camera.update_gl_modelview_matrix();

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

    if (dude.entered_door())
    {
        return &game_loop._states.main_menu;
    }

    auto& cheat_console = registry.get<prefabs::CheatConsoleComponent>(_cheat_console);
    if (cheat_console.is_state_change_requested()) {
        return game_loop.get_game_loop_state_ptr(cheat_console.get_requested_state());
    }

    return this;
}

void GameLoopSandboxState::enter(GameLoop& game_loop)
{
    log_info("Entered GameLoopSandboxState");

    auto& registry = EntityRegistry::instance().get_registry();

    auto& rendering_system = game_loop._rendering_system;

    Level::instance().get_tile_batch().clean();
    Level::instance().get_tile_batch().generate_frame();
    Level::instance().get_tile_batch().generate_cave_background();
    Level::instance().get_tile_batch().batch_vertices();
    Level::instance().get_tile_batch().add_render_entity(registry);

    auto& inventory = Inventory::instance();
    inventory.clear_items();
    game_loop._shopping_system = std::make_shared<ShoppingSystem>();

    float pos_x = Consts::LEVEL_WIDTH_TILES / 2;
    float pos_y = Consts::LEVEL_HEIGHT_TILES / 2;

    _main_dude = prefabs::MainDude::create(pos_x, pos_y);
    _pause_overlay = prefabs::PauseOverlay::create(game_loop._viewport, PauseOverlayComponent::Type::SCORES);
    _cheat_console = prefabs::CheatConsole::create(game_loop._viewport);
}

void GameLoopSandboxState::exit(GameLoop& game_loop)
{
    auto& registry = EntityRegistry::instance().get_registry();
    registry.clear();
}
