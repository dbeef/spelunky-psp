//
// Created by xdbeef on 24.02.18.
//

#ifndef SPELUNKYDS_CONSTS_H
#define SPELUNKYDS_CONSTS_H

#include <mm_types.h>
#include <nds/arm9/console.h>

#include "input/InputHandler.hpp"
#include "hud/Hud.hpp"
#include "camera/Camera.hpp"
#include "entities/main_dude/MainDude.hpp"
#include "entities/creatures/CreatureType.hpp"
#include "tiles/Level.hpp"
#include "memory/SpriteInfo.h"
#include "entities/main_dude/GameState.hpp"
#include "entities/decorations/_BaseDecoration.h"
#include "entities/treasures/_BaseTreasure.h"
#include "entities/items/_BaseItem.h"

#define GRAVITY_DELTA_SPEED 0.22 //FIXME isn't it mob-specific?
#define DEFAULT_DMA_CHANNEL 3

//https://stackoverflow.com/questions/21505021/extern-and-global-same-variable-in-same-file
//https://stackoverflow.com/questions/37323227/benefits-of-using-extern-declaration-in-same-file-as-definition
namespace global {

    void init_globals();

    extern MainDude *main_dude;
    extern InputHandler *input_handler;
    extern Camera *camera;
    extern Level *current_level;
    extern OAMManager *main_oam_manager;
    extern OAMManager *sub_oam_manager;
    extern Hud *hud;
    extern GameState *game_state;
    extern PrintConsole *print_console; //in-game console

    extern std::vector<BaseCreature *> creatures;
    extern std::vector<BaseCreature *> creatures_to_add;

    extern std::vector<BaseDecoration *> decorations;
    extern std::vector<BaseDecoration *> decorations_to_add;

    extern std::vector<BaseTreasure *> treasures;
    extern std::vector<BaseTreasure *> treasures_to_add;

    extern std::vector<BaseItem *> items;
    extern std::vector<BaseItem *> items_to_add;

    extern std::vector<SpriteInfo *> sprite_infos; //technically list of wrappers around OAM entries FIXME naming
    extern std::vector<CreatureType> killed_npcs; //list of NPCs killed in current level //FIXME should be SpriteType
    extern std::vector<TreasureType> collected_treasures; //list of loot collected in current level

    extern int bg_main_address; //technically, it's an id returned by oam init FIXME naming
    extern int bg_sub_address; //technically, it's an id returned by oam init FIXME naming
    extern double *timer; //global timer, updated in game loop FIXME can be int, /delta time/ is in milliseconds anyway
    extern double clean_unused_oam_timer; //every arbitrary amount of time OAM is checked for unused entities and cleaned off
    extern u16 current_map[4096]; //cave background with tiles rendered on it
    extern u16 base_map[4096]; //cave background only
    extern u16 *temp_map; //cave background only
    extern mm_sfxhand menu_music_handler; //integer identifier for menu music
    extern mm_sfxhand cave_music_handler; //integer identifier for cave level music
}

#endif //SPELUNKYDS_CONSTS_H
