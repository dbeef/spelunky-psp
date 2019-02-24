//
// Created by xdbeef on 06.05.18.
//

#ifndef SPELUNKYDS_GLOBALS_DEFINITIONS_H
#define SPELUNKYDS_GLOBALS_DEFINITIONS_H

#include <mm_types.h>
#include <nds/arm9/console.h>
#include "memory/SpritesheetType.hpp"
#include "entities/creatures/CreatureType.hpp"
#include "memory/SpriteInfo.h"
#include "hud/Hud.hpp"
#include "tiles/Level.hpp"
#include "entities/main_dude/MainDude.hpp"
#include "entities/main_dude/GameState.hpp"
#include "entities/decorations/_BaseDecoration.h"

namespace global {

    InputHandler *input_handler = nullptr;
    Camera *camera = nullptr;
    MainDude *main_dude = nullptr;
    Level *current_level = nullptr;
    OAMManager *main_oam_manager = nullptr;
    OAMManager *sub_oam_manager = nullptr;
    Hud *hud = nullptr;
    GameState *game_state = nullptr;
    PrintConsole *print_console = nullptr; //initialised in console_utils.h call from main

    //https://stackoverflow.com/questions/8036474/when-vectors-are-allocated-do-they-use-memory-on-the-heap-or-the-stack
    std::vector<BaseCreature *> creatures;
    std::vector<BaseCreature *> creatures_to_add;
    std::vector<BaseDecoration *> decorations;
    std::vector<BaseDecoration *> decorations_to_add;
    std::vector<BaseTreasure *> treasures;
    std::vector<BaseTreasure *> treasures_to_add;
    std::vector<BaseItem *> items;
    std::vector<BaseItem *> items_to_add;

    std::vector<SpriteInfo *> sprite_infos;

    std::vector<CreatureType> killed_npcs;
    std::vector<TreasureType > collected_treasures;

    int bg_main_address{};
    int bg_sub_address{};
    double *timer{};
    double clean_unused_oam_timer{};
    mm_sfxhand menu_music_handler{};
    mm_sfxhand cave_music_handler{};

    u16 current_map[4096];
    u16 *temp_map = new u16[4096];
}
#endif //SPELUNKYDS_GLOBALS_DEFINITIONS_H
