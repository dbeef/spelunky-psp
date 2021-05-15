#include "populator/Populator.hpp"
#include "prefabs/main-dude/MainDude.hpp"
#include "prefabs/ui/LevelSummaryOverlay.hpp"
#include "prefabs/npc/Damsel.hpp"
#include "prefabs/npc/DamselScript.hpp"

#include "game-loop/GameLoopLevelSummaryState.hpp"
#include "game-loop/GameLoop.hpp"

#include "components/specialized/LevelSummaryOverlayComponent.hpp"
#include "components/specialized/MainDudeComponent.hpp"

#include "system/RenderingSystem.hpp"
#include "system/DisposingSystem.hpp"
#include "system/ScriptingSystem.hpp"
#include "system/PhysicsSystem.hpp"
#include "system/AnimationSystem.hpp"
#include "system/ItemSystem.hpp"
#include "system/ParticleSystem.hpp"

#include "EntityRegistry.hpp"
#include "logger/log.h"
#include "ModelViewCamera.hpp"
#include "ScreenSpaceCamera.hpp"
#include "CameraType.hpp"
#include "Level.hpp"
#include "other/Inventory.hpp"

GameLoopBaseState *GameLoopLevelSummaryState::update(GameLoop& game_loop, uint32_t delta_time_ms)
{
    auto& registry = EntityRegistry::instance().get_registry();

    auto& rendering_system = game_loop._rendering_system;
    auto& scripting_system = game_loop._scripting_system;
    auto& physics_system = game_loop._physics_system;
    auto& animation_system = game_loop._animation_system;
    auto& item_system = game_loop._item_system;
    auto& particle_system = game_loop._particle_system;
    auto& disposing_system = game_loop._disposing_system;

    rendering_system->update(delta_time_ms);
    scripting_system->update(delta_time_ms);
    physics_system->update(delta_time_ms);
    animation_system->update(delta_time_ms);
    item_system->update(delta_time_ms);
    disposing_system->update(delta_time_ms);
    particle_system->update(delta_time_ms);

    auto& dude = registry.get<MainDudeComponent>(_main_dude);

    if (dude.entered_door())
    {
        return &game_loop._states.playing;
    }

    if (_damsel != entt::null)
    {
        auto &dude_position = registry.get<PositionComponent>(_main_dude);
        auto &dude_physics = registry.get<PhysicsComponent>(_main_dude);

        auto &damsel_position = registry.get<PositionComponent>(_damsel);
        auto &damsel_physics = registry.get<PhysicsComponent>(_damsel);

        if (dude_physics.is_collision(damsel_physics, damsel_position, dude_position))
        {
            auto& damsel_scripting_component = registry.get<ScriptingComponent>(_damsel);
            auto* damsel_script = damsel_scripting_component.get<prefabs::DamselScript>();

            damsel_script->enter_smooching_state(_damsel);
            Inventory::instance().add_hearts(1);
            _damsel = entt::null;
        }
    }

    return this;
}

void GameLoopLevelSummaryState::enter(GameLoop& game_loop)
{
    log_info("Entered GameLoopLevelSummaryState");

    auto& registry = EntityRegistry::instance().get_registry();
    auto& rendering_system = game_loop._rendering_system;

    Level::instance().get_tile_batch().generate_frame();
    Level::instance().get_tile_batch().initialise_tiles_from_splash_screen(SplashScreenType::LEVEL_SUMMARY);
    Level::instance().get_tile_batch().generate_cave_background();
    Level::instance().get_tile_batch().batch_vertices();
    Level::instance().get_tile_batch().add_render_entity(registry);

    prefabs::LevelSummaryOverlay::create(game_loop._viewport, game_loop._level_summary_tracker);

    // Splash screens are copied into the [0, 0] position (left-upper corner), center on them:
    auto &model_view_camera = game_loop._rendering_system->get_model_view_camera();
    model_view_camera.set_x_not_rounded(game_loop._viewport->get_width_world_units() / 4.0f);
    model_view_camera.set_y_not_rounded(game_loop._viewport->get_height_world_units() / 4.0f);
    model_view_camera.update_gl_modelview_matrix();

    // Update main dude:
    MapTile* entrance = nullptr;
    Level::instance().get_tile_batch().get_first_tile_of_given_type(MapTileType::ENTRANCE, entrance);
    assert(entrance);

    float pos_x = entrance->x + (MapTile::PHYSICAL_WIDTH / 2.0f);
    float pos_y = entrance->y + (MapTile::PHYSICAL_HEIGHT / 2.0f);

    // Update main dude:
    _main_dude = prefabs::MainDude::create(pos_x, pos_y);
    auto& dude = registry.get<MainDudeComponent>(_main_dude);
    dude.enter_level_summary_state();

    Populator().generate_inventory_items(_main_dude);

    auto& damsel_rescued = game_loop._states.playing.is_damsel_rescued();
    if (damsel_rescued)
    {
        _damsel = prefabs::Damsel::create(damsel_rescued, pos_x + MapTile::PHYSICAL_WIDTH * 8, pos_y);
    }

    rendering_system->sort();
}

void GameLoopLevelSummaryState::exit(GameLoop& game_loop)
{
    auto& registry = EntityRegistry::instance().get_registry();
    registry.clear();
}
