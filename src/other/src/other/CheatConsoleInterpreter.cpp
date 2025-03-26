#include "other/CheatConsoleInterpreter.h"
// #include "game-loop/GameLoop.hpp"
#include "prefabs/ui/CheatConsoleWindow.hpp"
#include "patterns/Singleton.hpp"
#include "components/generic/PositionComponent.hpp"
// #include "populator/NpcFactory.hpp"
// #include "populator/ItemFactory.hpp"
// #include "populator/LootFactory.hpp"
#include "EntityRegistry.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "prefabs/items/Arrow.hpp"
#include "prefabs/npc/Bat.hpp"
#include "prefabs/npc/BlueFrog.hpp"
#include "prefabs/npc/Caveman.hpp"
#include "prefabs/npc/RedFrog.hpp"
#include "prefabs/npc/Shopkeeper.hpp"
#include "prefabs/npc/Skeleton.hpp"
#include "prefabs/npc/Snake.hpp"
#include "prefabs/npc/Spider.hpp"
#include "prefabs/items/Bomb.hpp"
#include "prefabs/items/Cape.hpp"
#include "prefabs/items/Chest.hpp"
#include "prefabs/items/Crate.hpp"
#include "prefabs/items/Jar.hpp"
#include "prefabs/items/Jetpack.hpp"
#include "prefabs/items/Pistol.hpp"
#include "prefabs/items/Rock.hpp"
#include "prefabs/items/Rope.hpp"
#include "prefabs/items/Shotgun.hpp"
#include "prefabs/items/Skull.hpp"
#include "prefabs/items/Whip.hpp"
#include "prefabs/items/SpikeShoes.hpp"
#include "prefabs/items/SpringShoes.hpp"
#include "prefabs/items/Mitt.hpp"
#include "prefabs/items/Glove.hpp"
#include "prefabs/items/Compass.hpp"
#include "prefabs/items/BombBag.hpp"
#include "prefabs/items/RopePile.hpp"
#include "prefabs/items/GoldenIdol.hpp"
#include "prefabs/items/Flare.hpp"

namespace {
    namespace NpcFactory {
        entt::entity make(NpcType npc_type) {
            switch (npc_type) {
                case NpcType::SNAKE: return prefabs::Snake::create();
                case NpcType::BAT: return prefabs::Bat::create();
                case NpcType::CAVEMAN: return prefabs::Caveman::create();
                case NpcType::SPIDER: return prefabs::Spider::create();
                case NpcType::SKELETON: return prefabs::Skeleton::create();
                case NpcType::SHOPKEEPER: return prefabs::Shopkeeper::create();
                case NpcType::BLUE_FROG: return prefabs::BlueFrog::create();
                case NpcType::RED_FROG: return prefabs::RedFrog::create();
                default: {
                    assert(false);
                    return {};
                }
            }
        }

        entt::entity make(NpcType npc_type, float pos_x, float pos_y) {
            const auto out_entity = make(npc_type);
            auto &registry = EntityRegistry::instance().get_registry();
            auto &position = registry.get<PositionComponent>(out_entity);
            position.x_center = pos_x;
            position.y_center = pos_y;
            return out_entity;
        }
    }

    namespace LootFactory {
        entt::entity make(LootType loot_type) {
            switch (loot_type) {
                case LootType::NOTHING: break;
                case LootType::ANY: break;
                case LootType::SHOP_ITEM: break;
                case LootType::GOLDEN_IDOL: break;
            }
            assert(false);
            return {};
        }

        entt::entity make(LootType loot_type, float pos_x, float pos_y) {
            const auto out_entity = make(loot_type);
            auto &registry = EntityRegistry::instance().get_registry();
            auto &position = registry.get<PositionComponent>(out_entity);
            position.x_center = pos_x;
            position.y_center = pos_y;
            return out_entity;
        }
    }

    namespace ItemFactory {
        entt::entity make(ItemType item_type) {
            switch (item_type) {
                case ItemType::ARROW: return prefabs::Arrow::create();
                case ItemType::BOMB: return prefabs::Bomb::create();
                case ItemType::CAPE: return prefabs::Cape::create();
                case ItemType::CHEST: return prefabs::Chest::create();
                case ItemType::CRATE: return prefabs::Crate::create();
                case ItemType::JAR: return prefabs::Jar::create();
                case ItemType::JETPACK: return prefabs::Jetpack::create();
                case ItemType::PISTOL: return prefabs::Pistol::create();
                case ItemType::ROCK: return prefabs::Rock::create();
                case ItemType::ROPE: return prefabs::Rope::create();
                case ItemType::SHOTGUN: return prefabs::Shotgun::create();
                case ItemType::SKULL: return prefabs::Skull::create();
                case ItemType::WHIP: return prefabs::Whip::create();
                case ItemType::SPIKE_SHOES: return prefabs::SpikeShoes::create();
                case ItemType::SPRING_SHOES: return prefabs::SpringShoes::create();
                case ItemType::MITT: return prefabs::Mitt::create();
                case ItemType::GLOVE: return prefabs::Glove::create();
                case ItemType::COMPASS: return prefabs::Compass::create();
                case ItemType::BOMB_BAG: return prefabs::BombBag::create();
                case ItemType::ROPE_PILE: return prefabs::RopePile::create();
                case ItemType::GOLDEN_IDOL: return prefabs::GoldenIdol::create();
                case ItemType::FLARE: return prefabs::Flare::create();
            }
            assert(false);
            return {};
        }

        entt::entity make(ItemType item_type, float pos_x, float pos_y) {
            const auto out_entity = make(item_type);
            auto &registry = EntityRegistry::instance().get_registry();
            auto &position = registry.get<PositionComponent>(out_entity);
            position.x_center = pos_x;
            position.y_center = pos_y;
            return out_entity;
        }
    }
}

template<class Enumerator, class Enumerator_t>
std::map<std::string, Enumerator> populate_string_to_enum_map() {
    std::map<std::string, Enumerator> out;
    Enumerator_t enumerator_index = 0;
    while (enumerator_index < static_cast<Enumerator_t>(Enumerator::_SIZE)) {
        const auto enumerator = static_cast<Enumerator>(enumerator_index);
        out.emplace(to_string(enumerator), enumerator);
        enumerator_index++;
    }
    return out;
}

CheatConsoleInterpreter::CheatConsoleInterpreter() {
    _string_to_item_type_map = populate_string_to_enum_map<ItemType, ItemType_t>();
    _string_to_npc_type_map = populate_string_to_enum_map<NpcType, NpcType_t>();
    _string_to_loot_type_map = populate_string_to_enum_map<LootType, LootType_t>();
    _string_to_game_loop_state_map = populate_string_to_enum_map<GameLoopState, GameLoopState_t>();

    _spawn_command_handler = CommandHandler([&](const Command &command) {
        if (command.size() != 2 && (command.at(0) == "HELP" || command.at(0) == "SPAWN")) {
            return std::make_pair(false, "spawn <NpcType/ItemType>");
        }

        if (command.at(0) != "SPAWN") {
            return std::make_pair(false, "");
        }

        const auto &type = command.at(1);
        const auto npc_type_match = _string_to_npc_type_map.find(type);
        const auto item_type_match = _string_to_item_type_map.find(type);
        const auto loot_type_match = _string_to_loot_type_map.find(type);

        auto &registry = EntityRegistry::instance().get_registry();
        auto dudes = registry.view<MainDudeComponent>();
        assert(dudes.size() == 1);
        auto dude = dudes.front();
        auto &dude_position = registry.get<PositionComponent>(dude);

        const float offset_x = -2;
        const float offset_y = -2;

        if (npc_type_match != _string_to_npc_type_map.end()) {
            const NpcType npc_type = npc_type_match->second;
            NpcFactory::make(npc_type, dude_position.x_center + offset_x, dude_position.y_center + offset_y);
            return std::make_pair(true, "spawning");
        }

        if (item_type_match != _string_to_item_type_map.end()) {
            const ItemType item_type = item_type_match->second;
            ItemFactory::make(item_type, dude_position.x_center + offset_x, dude_position.y_center + offset_y);
            return std::make_pair(true, "spawning");
        }

        if (loot_type_match != _string_to_loot_type_map.end()) {
            const LootType loot_type = loot_type_match->second;
            LootFactory::make(loot_type, dude_position.x_center + offset_x, dude_position.y_center + offset_y);
            return std::make_pair(true, "spawning");
        }

        return std::make_pair(true, "failed to match <NpcType/ItemType>");
    });

    _enter_command_handler = CommandHandler([this](const Command &command) {
        if (command.size() != 2 && (command.at(0) == "HELP" || command.at(0) == "ENTER")) {
            return std::make_pair(false, "enter <GameLoop::State>");
        }

        if (command.at(0) != "ENTER") {
            return std::make_pair(false, "");
        }

        auto &registry = EntityRegistry::instance().get_registry();
        auto cheat_consoles = registry.view<prefabs::CheatConsoleWindowComponent>();
        assert(cheat_consoles.size() == 1);
        auto cheat_console = cheat_consoles.front();
        auto &cheat_console_component = registry.get<prefabs::CheatConsoleWindowComponent>(cheat_console);

        const auto &requested_game_loop_state = command.at(1);
        const auto game_loop_state_match = _string_to_game_loop_state_map.find(requested_game_loop_state);

        if (game_loop_state_match == _string_to_game_loop_state_map.end()) {
            return std::make_pair(true, "failed to match <GameLoopState>");
        }

        cheat_console_component.request_state_change(game_loop_state_match->second);
        return std::make_pair(true, "entering");
    });

    _quit_command_handler = CommandHandler([this](const Command &command) {
        if (command.size() != 1 || (command.at(0) != "QUIT" && command.at(0) != "EXIT")) {
            return std::make_pair(false, "");
        }

        auto &registry = EntityRegistry::instance().get_registry();
        auto cheat_consoles = registry.view<prefabs::CheatConsoleWindowComponent>();
        assert(cheat_consoles.size() == 1);
        auto cheat_console = cheat_consoles.front();
        auto &cheat_console_component = registry.get<prefabs::CheatConsoleWindowComponent>(cheat_console);
        cheat_console_component.request_state_change(GameLoopState::QUITTING);

        return std::make_pair(true, "quitting");
    });
}

const CheatConsoleInterpreter::CommandHandler &CheatConsoleInterpreter::get_spawn_command_handler() const {
    return _spawn_command_handler;
}

const CheatConsoleInterpreter::CommandHandler &CheatConsoleInterpreter::get_enter_command_handler() const {
    return _enter_command_handler;
}

const CheatConsoleInterpreter::CommandHandler &CheatConsoleInterpreter::get_quit_command_handler() const {
    return _quit_command_handler;
}
