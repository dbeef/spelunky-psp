#include "other/CheatConsoleInterpreter.h"
#include "game-loop/GameLoop.hpp"
#include "prefabs/ui/CheatConsole.hpp"
#include "patterns/Singleton.hpp"
#include "components/generic/PositionComponent.hpp"
#include "populator/NpcFactory.hpp"
#include "populator/ItemFactory.hpp"
#include "populator/LootFactory.hpp"
#include "EntityRegistry.hpp"
#include "components/specialized/MainDudeComponent.hpp"

template<class Enumerator, class Enumerator_t>
std::map<std::string, Enumerator> populate_string_to_enum_map()
{
    std::map<std::string, Enumerator> out;
    Enumerator_t enumerator_index = 0;
    while (enumerator_index < static_cast<Enumerator_t>(Enumerator::_SIZE)) {
        const auto enumerator = static_cast<Enumerator>(enumerator_index);
        out.emplace(to_string(enumerator), enumerator);
        enumerator_index++;
    }
    return out;
}

CheatConsoleInterpreter::CheatConsoleInterpreter()
{
    _string_to_item_type_map = populate_string_to_enum_map<ItemType, ItemType_t>();
    _string_to_npc_type_map = populate_string_to_enum_map<NpcType, NpcType_t>();
    _string_to_loot_type_map = populate_string_to_enum_map<LootType, LootType_t>();
    _string_to_game_loop_state_map = populate_string_to_enum_map<GameLoopState, GameLoopState_t>();

    _spawn_command_handler = CommandHandler([&](const Command& command){
        if (command.size() != 2 && (command.at(0) == "HELP" || command.at(0) == "SPAWN")) {
            return std::make_pair(false, "spawn <NpcType/ItemType>");
        }

        if (command.at(0) != "SPAWN") {
            return std::make_pair(false, "");
        }

        const auto& type = command.at(1);
        const auto npc_type_match = _string_to_npc_type_map.find(type);
        const auto item_type_match = _string_to_item_type_map.find(type);
        const auto loot_type_match = _string_to_loot_type_map.find(type);

        auto& registry = EntityRegistry::instance().get_registry();
        auto dudes = registry.view<MainDudeComponent>();
        assert(dudes.size() == 1);
        auto dude = dudes.front();
        auto &dude_position = registry.get<PositionComponent>(dude);

        const float offset_x = -2;
        const float offset_y = -2;

        if (npc_type_match != _string_to_npc_type_map.end())
        {
            const NpcType npc_type = npc_type_match->second;
            NpcFactory::make(npc_type, dude_position.x_center + offset_x, dude_position.y_center + offset_y);
            return std::make_pair(true, "spawning");
        }

        if (item_type_match != _string_to_item_type_map.end())
        {
            const ItemType item_type = item_type_match->second;
            ItemFactory::make(item_type, dude_position.x_center + offset_x, dude_position.y_center + offset_y);
            return std::make_pair(true, "spawning");
        }

        if (loot_type_match != _string_to_loot_type_map.end())
        {
            const LootType loot_type = loot_type_match->second;
            LootFactory::make(loot_type, dude_position.x_center + offset_x, dude_position.y_center + offset_y);
            return std::make_pair(true, "spawning");
        }

        return std::make_pair(true, "failed to match <NpcType/ItemType>");
    });

    _enter_command_handler = CommandHandler ([this](const Command& command){
        if (command.size() != 2 && (command.at(0) == "HELP" || command.at(0) == "ENTER")) {
            return std::make_pair(false, "enter <GameLoop::State>");
        }

        if (command.at(0) != "ENTER") {
            return std::make_pair(false, "");
        }

        auto& registry = EntityRegistry::instance().get_registry();
        auto cheat_consoles = registry.view<prefabs::CheatConsoleComponent>();
        assert(cheat_consoles.size() == 1);
        auto cheat_console = cheat_consoles.front();
        auto& cheat_console_component = registry.get<prefabs::CheatConsoleComponent>(cheat_console);

        const auto& requested_game_loop_state = command.at(1);
        const auto game_loop_state_match = _string_to_game_loop_state_map.find(requested_game_loop_state);

        if (game_loop_state_match == _string_to_game_loop_state_map.end())
        {
            return std::make_pair(true, "failed to match <GameLoopState>");
        }

        cheat_console_component.request_state_change(game_loop_state_match->second);
        return std::make_pair(true, "entering");
    });
}

const CheatConsoleInterpreter::CommandHandler& CheatConsoleInterpreter::get_spawn_command_handler() const
{
    return _spawn_command_handler;
}

const CheatConsoleInterpreter::CommandHandler& CheatConsoleInterpreter::get_enter_command_handler() const
{
    return _enter_command_handler;
}