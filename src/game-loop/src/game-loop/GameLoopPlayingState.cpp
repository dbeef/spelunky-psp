#include "prefabs/ui/DeathOverlay.hpp"
#include "prefabs/ui/HudOverlay.hpp"
#include "prefabs/ui/PauseOverlay.hpp"
#include "prefabs/npc/ShopkeeperScript.hpp"
#include "prefabs/main-dude/MainDude.hpp"

#include "game-loop/GameLoopPlayingState.hpp"
#include "game-loop/GameLoop.hpp"

#include "EntityRegistry.hpp"

#include "components/specialized/HudOverlayComponent.hpp"
#include "components/specialized/PauseOverlayComponent.hpp"
#include "components/specialized/DeathOverlayComponent.hpp"
#include "components/specialized/LevelSummaryTracker.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
#include "components/generic/CollectibleComponent.hpp"

#include "system/RenderingSystem.hpp"
#include "system/DamageSystem.hpp"
#include "system/ItemSystem.hpp"
#include "system/CollectibleSystem.hpp"
#include "system/ScriptingSystem.hpp"
#include "system/ShoppingSystem.hpp"
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
#include "prefabs/ui/CheatConsole.hpp"

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
    auto& damage_system = game_loop._damage_system;
    auto& shopping_system = game_loop._shopping_system;

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
        damage_system->update(delta_time_ms);
        shopping_system->update(delta_time_ms);
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

    auto& cheat_console = registry.get<prefabs::CheatConsoleComponent>(_cheat_console);
    switch (cheat_console.state)
    {
        case GameLoop::State::SCORES:
        {
            cheat_console.state = GameLoop::State::CURRENT;
            return &game_loop._states.scores;
        }
        case GameLoop::State::MAIN_MENU:
        {
            cheat_console.state = GameLoop::State::CURRENT;
            return &game_loop._states.main_menu;
        }
        case GameLoop::State::PLAYING:
        {
            cheat_console.state = GameLoop::State::CURRENT;
            return &game_loop._states.playing;
        }
        default: {}
    }

    return this;
}

void GameLoopPlayingState::enter(GameLoop& game_loop)
{
    log_info("Entered GameLoopPlayingState");
    std::srand(game_loop._time_elapsed_ms);

    auto& registry = EntityRegistry::instance().get_registry();

    Audio::instance().play(MusicType::CAVE);

    TileBatch::LevelGeneratorParams generator_params;
    generator_params.shopkeeper_robbed = game_loop._shopping_system->is_robbed();

    Level::instance().get_tile_batch().generate_new_level_layout(generator_params);
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
    _cheat_console = prefabs::CheatConsole::create(game_loop._viewport);

    game_loop._rendering_system->sort();

    auto& dude = registry.get<MainDudeComponent>(_main_dude);
    dude.add_observer(this);

    // Subscribe HUD on main-dude's ItemCarrierComponent events:
    auto& hud_component = registry.get<HudOverlayComponent>(_hud);
    auto& item_component = registry.get<ItemCarrierComponent>(_main_dude);
    item_component.add_observer(hud_component.get_item_observer());

    auto& death = registry.get<DeathOverlayComponent>(_death_overlay);
    death.disable_input();

    Populator populator;
    populator.generate_loot(game_loop._shopping_system->is_robbed());
    populator.generate_npc(is_damsel_rescued(), game_loop._shopping_system->is_robbed());
    populator.generate_inventory_items(_main_dude);

    // Wire subjects with observers:

    for (const auto& collectible_entity : populator.get_collectibles())
    {
        auto& collectible = registry.get<CollectibleComponent>(collectible_entity);
        collectible.add_observer(game_loop._level_summary_tracker.get());
    }

    for (const auto& npc_entity : populator.get_npcs())
    {
        auto& hitpoints = registry.get<HitpointComponent>(npc_entity);
        hitpoints.add_observer(game_loop._level_summary_tracker.get());
    }

    for (const auto& shopkeeper : populator.get_shopkeepers())
    {
        auto &scripting_component = registry.get<ScriptingComponent>(shopkeeper);
        auto *shopkeeper_script = scripting_component.get<prefabs::ShopkeeperScript>();
        game_loop._shopping_system->add_observer(shopkeeper_script->get_thievery_observer());
        shopkeeper_script->add_observer(game_loop._shopping_system.get());

        if (game_loop._shopping_system->is_robbed())
        {
            shopkeeper_script-> get_angry(shopkeeper);
        }
    }

    game_loop._level_summary_tracker->entered_new_level();

    _special_events.damsel_rescued = false;

    // Subscribe HUD on main-dude's wallet:

    auto& item_carrier = registry.get<ItemCarrierComponent>(_main_dude);
    const auto wallet_entity = item_carrier.get_item(ItemType::WALLET);

    if (wallet_entity != entt::null)
    {
        auto& wallet_scripting_component = registry.get<ScriptingComponent>(wallet_entity);
        auto* wallet_script = wallet_scripting_component.get<prefabs::WalletScript>();

        auto& hud_overlay = registry.get<HudOverlayComponent>(_hud);
        wallet_script->add_observer(static_cast<Observer<ShoppingTransactionEvent>*>(&hud_overlay));
    }
}

void GameLoopPlayingState::exit(GameLoop& game_loop)
{
    auto& registry = EntityRegistry::instance().get_registry();
    Audio::instance().stop();

    auto& dude = registry.get<MainDudeComponent>(_main_dude);
    auto& dude_item_carrier_component = registry.get<ItemCarrierComponent>(_main_dude);

    // Save collected item types in inventory which is persistent between the levels:
    auto& inventory = Inventory::instance();
    inventory.set_items(dude_item_carrier_component.get_items());

    game_loop._shopping_system->remove_all_observers();

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
            if (_hud == entt::null)
            {
                break;
            }

            // TODO: Maybe just remove components when they are no longer needed?
            auto& death = registry.get<DeathOverlayComponent>(_death_overlay);
            auto& pause = registry.get<PauseOverlayComponent>(_pause_overlay);

            death.launch();
            death.enable_input();

            pause.unpause();
            pause.hide(registry);
            pause.disable_input();

            registry.view<ItemComponent>().each([&registry](entt::entity item_entity, ItemComponent& item)
            {
                if (item.get_type() == ItemType::COMPASS)
                {
                    auto& compass_scripting_component = registry.get<ScriptingComponent>(item_entity);
                    auto* compass_script = compass_scripting_component.get<prefabs::CompassScript>();
                    compass_script->remove_all_observers();
                }
            });

            auto& item_carrier_component = registry.get<ItemCarrierComponent>(_main_dude);
            auto& hud_overlay_component = registry.get<HudOverlayComponent>(_hud);

            item_carrier_component.remove_observer(hud_overlay_component.get_item_observer());
            const auto wallet_entity = item_carrier_component.get_item(ItemType::WALLET);

            if (wallet_entity != entt::null)
            {
                auto& wallet_scripting_component = registry.get<ScriptingComponent>(wallet_entity);
                auto* wallet_script = wallet_scripting_component.get<prefabs::WalletScript>();
                wallet_script->remove_observer(static_cast<Observer<ShoppingTransactionEvent>*>(&hud_overlay_component));
            }

            registry.destroy(_hud);
            _hud = entt::null;

            break;
        }
        default: break;
    }
}
