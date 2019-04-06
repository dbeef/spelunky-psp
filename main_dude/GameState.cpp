//
// Created by xdbeef on 27.05.18.
//

#include <maxmod9.h>
#include <cstdio>
#include "GameState.hpp"
#include "../../GlobalsDeclarations.hpp"
#include "../../GameLoop.hpp"
#include "../../tiles/LevelRenderingUtils.hpp"
#include "../../tiles/SplashScreenType.hpp"
#include "../../memory/OamUtils.hpp"
#include "../../../build/soundbank.h"
#include "../creatures/Damsel.hpp"
#include "../../entities/decorations/Smooch.hpp"
#include "../../sound/SoundUtils.hpp"
#include "../../BaseMap.hpp"
#include "../../tiles/LevelGenerator.hpp"
#include "../../tiles/PopulatingUtils.hpp"

void GameState::reset_main_dude() {
    global::main_dude->carrying_spring_shoes = false;
    global::main_dude->carrying_compass = false;
    global::main_dude->carrying_glove = false;
    global::main_dude->carrying_mitt = false;
    global::main_dude->carrying_spike_shoes = false;
    global::main_dude->carrying_cape = false;
    global::main_dude->carrying_shotgun = false;
    global::main_dude->carrying_pistol = false;
    global::main_dude->carrying_jetpack = false;
    global::main_dude->dead = false;
    global::main_dude->stunned = false;
    global::main_dude->hanging_on_tile_right = false;
    global::main_dude->hanging_on_tile_left = false;
}

void GameState::start_new_game() {

    reset_main_dude();
    global::hud->hearts = 4;
    global::hud->ropes = 4;
    global::hud->bombs = 4;
    global::hud->money = 0;
    global::hud->items_offset_y = 7;
    global::hud->game_over_timer = 0;
    global::hud->disable_all_prompts();
    global::hud->set_hud_sprites_attributes();
    global::hud->draw_level_hud();

}

void GameState::start_main_menu() {

    global::main_dude->climbing = false;
    global::camera->follow_main_dude = false;
    global::game_state->in_main_menu = true;
    global::game_state->levels_transition_screen = false;
    global::game_state->scores_screen = false;
    populate_main_menu();
}

void GameState::start_scores() {

    reset_main_dude();

    global::game_state->scores_screen = true;
    global::hud->draw_scores();
    global::camera->follow_main_dude = false;
}

void GameState::start_level_transition_screen() {
    global::hud->total_time_spent += global::hud->time_spent_on_level;
    global::hud->level++;

    global::game_state->levels_transition_screen = true;
    global::game_state->splash_screen = true;

    global::input_handler->stop_handling = true;
    global::input_handler->l_bumper_held = true;
    global::input_handler->right_key_held = true;

    global::camera->follow_main_dude = false;
    consoleClear();
    global::hud->draw_on_level_done();


    if (global::game_state->damsels_rescued_this_level > 0) {
        global::hud->hearts += global::game_state->damsels_rescued_this_level;
        global::game_state->damsels_rescued_this_level = 0;
        global::game_state->smooching = true;

        Damsel *damsel = new Damsel(0, 0);
        damsel->call_for_help = false;
        global::creatures.push_back(damsel);

        MapTile *entrance;
        global::current_level->get_first_tile_of_given_type(MapTileType::ENTRANCE, entrance);
        if (entrance != nullptr) {
            damsel->_x = (entrance->x * 16) + 7 * 16;
            damsel->_y = entrance->y * 16;
            //144
            //448
        }

    }

}

void GameState::start_next_level() {

    global::hud->items_offset_y = 7;
    global::main_dude->holding_item = false;
    global::main_dude->spawn_carried_items();
    global::hud->init_sprites();
    populate_cave_npcs();
    populate_cave_moniez();
    global::game_state->levels_transition_screen = false;
    global::game_state->in_main_menu = false;
    global::killed_npcs.clear();
    global::collected_treasures.clear();
    global::hud->money_on_this_level = 0;
    global::hud->draw_level_hud();

}

void GameState::handle_changing_screens() {

    if ((global::main_dude->dead && global::input_handler->y_key_down) ||
        (global::main_dude->animFrame >= 16 && !global::game_state->splash_screen)) {

        global::main_dude->animFrame = 0;

//        printf("BEFORE DMA COPY\n");
//        for(int a =0;a < 1*60;a++)
//            swiWaitForVBlank();

        dmaCopyHalfWords(DEFAULT_DMA_CHANNEL, global::base_map, global::current_map, sizeof(global::base_map));

//        printf("BEFORE NEW LAYOUT\n");
//        for(int a =0;a < 1*60;a++)
//            swiWaitForVBlank();

        generate_new_level_layout();

        if (global::game_state->in_main_menu || global::game_state->levels_transition_screen) {

            //next level or starting game

            if (global::game_state->in_main_menu) {
                sound::stop_menu_music();
                sound::start_cave_music();
            } else
                sound::stop_cave_music();


//            printf("BEFORE GENERATE FRAME\n");
//            for(int a =0;a < 1*60;a++)
//                swiWaitForVBlank();

            global::current_level->generate_frame();

//            printf("BEFORE GENERATE ROOMS\n");
//            for(int a =0;a < 1*60;a++)
//                swiWaitForVBlank();

            global::current_level->initialise_tiles_from_room_layout();
            set_position_to(MapTileType::ENTRANCE);


//            printf("AFTER GENERATE ROOMS\n");
//            for(int a =0;a < 1*60;a++)
//                swiWaitForVBlank();

        } else {

            global::current_level->clean_map_layout();

            //flushing the buffer so it wouldn't update in the scores/main menu/transition screen if != 0
            global::hud->money_on_this_level += global::hud->dollars_buffer;
            global::hud->dollars_buffer = 0;

            //splash screen; scores or level transition

            if (global::game_state->scores_screen) {

                global::game_state->robbed_killed_shopkeeper = false;

                sound::stop_cave_music();
                sound::start_menu_music();

                global::current_level->initialise_tiles_from_splash_screen(SplashScreenType::MAIN_MENU_UPPER);
                global::current_level->initialise_tiles_from_splash_screen(SplashScreenType::MAIN_MENU_LOWER);
//                set_position_to(MapTileType::ENTRANCE);
                global::main_dude->_x = 113;
                global::main_dude->_y = 288;
                global::camera->follow_main_dude = true;
                global::camera->instant_focus();

            } else if (global::main_dude->dead) {
                global::current_level->initialise_tiles_from_splash_screen(SplashScreenType::SCORES_UPPER);
                global::current_level->initialise_tiles_from_splash_screen(SplashScreenType::SCORES_LOWER);
                set_position_to(MapTileType::EXIT);
            } else {

                global::current_level->initialise_tiles_from_splash_screen(SplashScreenType::ON_LEVEL_DONE_UPPER);
                global::current_level->initialise_tiles_from_splash_screen(SplashScreenType::ON_LEVEL_DONE_LOWER);
                set_position_to(MapTileType::ENTRANCE);
//                global::main_dude->x = 32;
//                global::main_dude->y = 280;
//                global::camera->follow_main_dude = true;
//                global::camera->instant_focus();
            }

        }

        //

//        printf("BEFORE TILES TO MAP\n");
//        for(int a =0;a < 1*60;a++)
//            swiWaitForVBlank();

        global::current_level->write_tiles_to_map();


//        printf("BEFORE SECTORIZE\n");
//        for(int a =0;a < 1*60;a++)
//            swiWaitForVBlank();

        sectorize_map();

//        printf("BEFORE COPY 1\n");
//        for(int a =0;a < 1*60;a++)
//            swiWaitForVBlank();

        dmaCopyHalfWords(DEFAULT_DMA_CHANNEL, global::current_map, bgGetMapPtr(global::bg_main_address),
                         sizeof(global::current_map));


//        printf("BEFORE COPY 2\n");
//        for(int a =0;a < 1*60;a++)
//            swiWaitForVBlank();

        dmaCopyHalfWords(DEFAULT_DMA_CHANNEL, global::current_map, bgGetMapPtr(global::bg_sub_address),
                         sizeof(global::current_map));
        //

//        printf("BEFORE DELETE ALL SPRITES\n");
//        for(int a =0;a < 1*60;a++)
//            swiWaitForVBlank();


        bool dead = global::main_dude->dead;
        int temp_x = global::main_dude->_x;
        int temp_y = global::main_dude->_y;

        //changing scene, so delete all MovingObjects you have, and according SpriteInfos
        oam_utils::delete_all_sprites();

        //new MainDude since we deleted it. TODO Don't push main dude to the global::entities and update it separately
        global::main_dude = new MainDude(224, 300);

        global::main_dude->dead = dead; //TODO Move this fields out to the game_state! or do the thing above, same result

        global::main_dude->_x = temp_x;
        global::main_dude->_y = temp_y;

//        global::main_dude->init();
//        global::entities.push_back(global::main_dude);

        consoleClear();

        if (global::game_state->in_main_menu || global::game_state->levels_transition_screen) {

            if (global::game_state->in_main_menu) {
                global::game_state->start_new_game();
            } else {
                sound::start_cave_music();
            }

            global::game_state->start_next_level(); //initializes hud entities

        } else {

            if (global::game_state->scores_screen)
                global::game_state->start_main_menu();
            else if (global::main_dude->dead) {
                global::game_state->start_scores();
            } else
                global::game_state->start_level_transition_screen();
        }

        global::main_dude->exiting_level = false;


    } else if (global::main_dude->animFrame >= 16 && global::game_state->splash_screen) {

        global::main_dude->main_sprite_info->entry->isHidden = true;
        global::main_dude->sub_sprite_info->entry->isHidden = true;
        global::input_handler->stop_handling = false;

        if (global::input_handler->y_key_down) {
            global::game_state->splash_screen = false;
        }

    }
}

/**
 * Sets main dude's position to the first tile of given type that occurs during iteration through the map
 */
void GameState::set_position_to(MapTileType t) {

    MapTile *entrance;
    global::current_level->get_first_tile_of_given_type(t, entrance);

    if (entrance != nullptr && entrance->exists) {
        global::main_dude->_x = entrance->x * 16;
        global::main_dude->_y = entrance->y * 16;
    } else {
        global::main_dude->_x = 0;
        global::main_dude->_y = 144;
    }

    global::camera->follow_main_dude = true;
    global::camera->instant_focus();

}

/**
 * TODO Move this to the Damsel class.
 */
void GameState::handle_transition_screen_smooch() {
    if (smooching) {
        if (144 - global::main_dude->_x <= 16) {
            smooch_timer += *global::timer;
            global::input_handler->right_key_held = false;
            if (!spawned_smooch) {
                mmEffect(SFX_XKISS);
                spawned_smooch = true;
                auto *s = new Smooch(144, 436);
                global::decorations_to_add.push_back(s);
            }
        }
        //144
        //448
    }
}