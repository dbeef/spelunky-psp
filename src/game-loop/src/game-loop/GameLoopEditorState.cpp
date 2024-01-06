#include <cmath>
#include "game-loop/GameLoopEditorState.hpp"
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
#include "prefabs/ui/CheatConsoleWindow.hpp"
#include "prefabs/ui/TileBrowserWindow.hpp"

GameLoopBaseState *GameLoopEditorState::update(GameLoop& game_loop, uint32_t delta_time_ms)
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
    auto& model_view_camera = game_loop._rendering_system->get_model_view_camera();

    auto& input = Input::instance();

    auto& viewport = game_loop._viewport;

    static float timer = 0;
    timer += delta_time_ms;
    if (timer > 10) {
        if (input.left().value()) {
            model_view_camera.set_x(model_view_camera.get_x() - 0.1f);
        }
        if (input.right().value()) {
            model_view_camera.set_x(model_view_camera.get_x() + 0.1f);
        }
        if (input.up().value()) {
            model_view_camera.set_y(model_view_camera.get_y() - 0.1f);
        }
        if (input.down().value()) {
            model_view_camera.set_y(model_view_camera.get_y() + 0.1f);
        }
//        float change = ((float)input.get_mouse_delta_x() / 10.0f);
//        if (change) {
//            log_info("%f", change);
//            model_view_camera.set_x(model_view_camera.get_x() + change);
//        }

        if (input.is_mouse_clicked())
        {
            log_info("click: %f %f", (float)input.get_mouse_x() / viewport->get_width_pixels(), (float)input.get_mouse_y() / viewport->get_height_pixels());
            log_info("camera: %f %f", model_view_camera.get_x(), model_view_camera.get_y());

            float target_pos_x = (model_view_camera.get_x() * 2) - ((float)viewport->get_width_world_units() / 2.0f);
            float click_x_normalized = (float)input.get_mouse_x() / viewport->get_width_pixels();
            target_pos_x += click_x_normalized * viewport->get_width_world_units();
            target_pos_x = std::floor(target_pos_x);

            float target_pos_y = (model_view_camera.get_y() * 2) - ((float)viewport->get_height_world_units() / 2.0f);
            float click_y_normalized = (float)input.get_mouse_y() / viewport->get_height_pixels();
            target_pos_y += click_y_normalized * viewport->get_height_world_units();
            target_pos_y = std::floor(target_pos_y);

            log_info("Clicked: %f %f",target_pos_x, target_pos_y);

            auto view = registry.view<prefabs::TileBrowserWindowComponent>();
            auto tiles = registry.get<prefabs::TileBrowserWindowComponent>(view.front());

            if (target_pos_x >= 0 && target_pos_y >= 0 && target_pos_x < Consts::ROOM_WIDTH_TILES && target_pos_y < Consts::ROOM_HEIGHT_TILES) {
                Level::instance().get_tile_batch().map_tiles[(int) target_pos_x][(int) target_pos_y]->map_tile_type = tiles.get_selected_tile_type();
                Level::instance().get_tile_batch().batch_vertices();
                Level::instance().get_tile_batch().add_render_entity(registry);
            }
        }

        timer = 0;
    }

//    model_view_camera.adjust_to_level_boundaries(Consts::LEVEL_WIDTH_TILES, Consts::LEVEL_HEIGHT_TILES);
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

    auto& cheat_console = registry.get<prefabs::CheatConsoleWindowComponent>(_cheat_console);
    if (cheat_console.is_state_change_requested()) {
        return game_loop.get_game_loop_state_ptr(cheat_console.get_requested_state());
    }

    return this;
}

void GameLoopEditorState::enter(GameLoop& game_loop)
{
    log_info("Entered GameLoopEditorState");

    auto& registry = EntityRegistry::instance().get_registry();
    auto& rendering_system = game_loop._rendering_system;

    Level::instance().recreate_tile_batch(Consts::ROOM_WIDTH_TILES, Consts::ROOM_HEIGHT_TILES);
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

    _pause_overlay = prefabs::PauseOverlay::create(game_loop._viewport, PauseOverlayComponent::Type::SCORES);
    _cheat_console = prefabs::CheatConsoleWindow::create(game_loop._viewport);
    _tile_browser = prefabs::TileBrowserWindow::create(game_loop._viewport);
}

void GameLoopEditorState::exit(GameLoop& game_loop)
{
    auto& registry = EntityRegistry::instance().get_registry();
    registry.clear();
}
