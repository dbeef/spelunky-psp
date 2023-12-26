#pragma once

#include <vector>
#include <string>
#include <functional>
#include <map>

#include "other/ItemType.hpp"
#include "other/NpcType.hpp"
#include "game-loop/GameLoopState.hpp"
#include "LootType.hpp"

class CheatConsoleInterpreter
{
public:
    using Command = std::vector<std::string>;
    using CommandHandlerResult = std::pair<bool, std::string>;
    using CommandHandler = std::function<CommandHandlerResult(const Command&)>;
    CheatConsoleInterpreter();

    const CommandHandler& get_spawn_command_handler() const;
    const CommandHandler& get_enter_command_handler() const;
private:
    std::map<std::string, ItemType> _string_to_item_type_map;
    std::map<std::string, NpcType> _string_to_npc_type_map;
    std::map<std::string, LootType> _string_to_loot_type_map;
    std::map<std::string, GameLoopState> _string_to_game_loop_state_map;

    CommandHandler _spawn_command_handler;
    CommandHandler _enter_command_handler;
};
