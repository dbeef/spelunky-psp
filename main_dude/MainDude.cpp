//
// Created by xdbeef on 24.02.18.
//

#include <nds.h>
#include <nds/arm9/sprite.h>
#include <maxmod9.h>
#include <cstdlib>

#include "MainDude.hpp"
#include "../../GlobalsDeclarations.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../tiles/LevelRenderingUtils.hpp"
#include "../../../build/gfx_spelunker.h"
#include "../../../build/soundbank.h"
#include "../../tiles/TileOrientation.hpp"
#include "../animations/FallPoof.hpp"
#include "../../sound/SoundUtils.hpp"
#include "../../memory/SpriteUtils.hpp"
#include "../../tiles/MapTile.hpp"
#include "../../tiles/Level.hpp"
#include "../../camera/Camera.hpp"
#include "../../memory/SpriteInfo.h"
#include "../items/Bomb.hpp"
#include "../../input/InputHandler.hpp"

// Called externally in game loop
void MainDude::handle_key_input() {

    if (!stunned && !exiting_level && !dead) {
        if (global::input_handler->b_key_down && time_since_last_jump > 100) {

            if (_bottom_collision || climbing) {
                if (carrying_spring_shoes)
                    _y_speed = -MAIN_DUDE_JUMP_SPEED * 1.65;
                else
                    _y_speed = -MAIN_DUDE_JUMP_SPEED;
                climbing = false;
                started_climbing_rope = false;
                started_climbing_ladder = false;
                can_climb_rope = false;
                time_since_last_jump = 0;

                mmEffect(SFX_XJUMP);
            } else if (_y_speed > 0 && carrying_cape) {
                using_cape = true;
            }
            if ((hanging_on_tile_left || hanging_on_tile_right) && hanging_timer > MAIN_DUDE_MIN_HANGING_TIME &&
                time_since_last_jump > 100) {

                mmEffect(SFX_XJUMP);

                _y_speed = -MAIN_DUDE_JUMP_SPEED;
                hanging_on_tile_left = false;
                hanging_on_tile_right = false;
                hanging_timer = 0;
                time_since_last_jump = 0;

            }
        }

        if (global::input_handler->b_key_down && time_since_last_jump > 100) {

            if (!climbing && carrying_jetpack && jetpack_fuel_counter > 0) {
                jumping_timer = 0;
                using_jetpack = true;
                _y_speed -= MAIN_DUDE_JUMP_SPEED;
                jetpack_fuel_counter--;
                mmEffect(SFX_XJETPACK);
                time_since_last_jump = 0;
            } else
                using_jetpack = false;

        }

        if (global::input_handler->y_key_down) {

            if (!stunned && !using_whip) {
                if (holding_item) {
                    throw_item();
                } else {
                    mmEffect(SFX_XWHIP);
                    using_whip = true;
                    animFrame = 0;
                }
            }

        }

        if (global::input_handler->x_key_down && global::hud->ropes > 0) {
            throw_rope();
        } else if (global::input_handler->a_key_down && !holding_item && global::hud->bombs > 0) {
            take_out_bomb();
        }

        if (global::input_handler->left_key_held) {

            sprite_state = Orientation::LEFT;
            hanging_on_tile_left = false;
            if (!(hanging_on_tile_right || hanging_on_tile_left) && !climbing)
                if (speed_inc_timer > MAIN_DUDE_X_SPEED_DELTA_TIME_MS) {
                    _x_speed -= MAIN_DUDE_X_SPEED_DELTA_VALUE;
                    speed_inc_timer = 0;
                }

        }

        if (global::input_handler->right_key_held) {

            sprite_state = Orientation::RIGHT;
            hanging_on_tile_right = false;
            if (!(hanging_on_tile_right || hanging_on_tile_left) && !climbing) {
                if (speed_inc_timer > MAIN_DUDE_X_SPEED_DELTA_TIME_MS) {
                    _x_speed += MAIN_DUDE_X_SPEED_DELTA_VALUE;
                    speed_inc_timer = 0;
                }
            }

        }

        int xx = floor_div(this->_x + 0.5 * _physical_width, TILE_W);
        int yy = floor_div(this->_y + 0.5 * _physical_height, TILE_H);

        current_x_in_tiles = xx;
        current_y_in_tiles = yy;

        if (global::input_handler->l_bumper_held || global::input_handler->up_key_held ||
            global::input_handler->down_key_held) {

            if (climbing) {

                if (global::game_state->in_main_menu && !global::game_state->exiting_game) {
                    if (_y <= 100) {
                        global::game_state->exiting_game = true;
                    }
                }

                climbing_timer += *global::timer;
                if (climbing_timer > 260) {
                    climbing_timer = 0;
                    climbing_sound++;
                    if (climbing_sound % 2 == 0)
                        mmEffect(SFX_XCLIMB1);
                    else
                        mmEffect(SFX_XCLIMB2);
                }
            } else {
                climbing_timer = 200;
            }

            MapTile *neighboringTiles[9] = {};
            Collisions::getNeighboringTiles(global::current_level->map_tiles, xx, yy, neighboringTiles);


            can_climb_ladder = neighboringTiles[CENTER] != nullptr &&
                               (neighboringTiles[CENTER]->mapTileType == MapTileType::LADDER ||
                                neighboringTiles[CENTER]->mapTileType == MapTileType::LADDER_DECK);

            can_climb_ladder = can_climb_ladder && global::input_handler->up_key_held;
            can_climb_rope = can_climb_rope && global::input_handler->up_key_held;

            exiting_level = neighboringTiles[CENTER] != nullptr &&
                            (neighboringTiles[CENTER]->mapTileType == MapTileType::EXIT) &&
                            global::input_handler->l_bumper_held;

            if (exiting_level) {

                mmEffect(SFX_XSTEPS);

                sound::stop_cave_music();

                _x = neighboringTiles[CENTER]->x * 16;
                _y = neighboringTiles[CENTER]->y * 16;

                animFrame = 0;
                animation_frame_timer = 0;
                _x_speed = 0;
                _y_speed = 0;

            }

            on_top_of_climbing_space = on_top_of_climbing_space ||
                                       (neighboringTiles[UP_MIDDLE] != nullptr &&
                                        neighboringTiles[UP_MIDDLE]->collidable);

            if (can_climb_ladder) {
                _x = neighboringTiles[CENTER]->x * 16;
            }

            if (can_climb_rope || can_climb_ladder) {

                climbing = true;
                jumping_timer = 0;
                _x_speed = 0;

                if (global::input_handler->up_key_held)
                    _y_speed = -1;

                if (can_climb_rope)
                    started_climbing_rope = true;
                else
                    started_climbing_ladder = true;
            }

            if (!can_climb_rope && climbing && on_top_of_climbing_space && !can_climb_ladder) {
                _y_speed = 0;
                jumping_timer = 0;
                _x_speed = 0;
            }

        } else if (climbing) {
            _y_speed = 0;
        }

        if (global::input_handler->down_key_held) {

            MapTile *neighboringTiles[9] = {};

            Collisions::getNeighboringTiles(global::current_level->map_tiles, xx, yy, neighboringTiles);
            can_climb_ladder = neighboringTiles[CENTER]->mapTileType == MapTileType::LADDER ||
                               neighboringTiles[CENTER]->mapTileType == MapTileType::LADDER_DECK;

            if (climbing) {
                can_climb_ladder = neighboringTiles[CENTER] != nullptr &&
                                   (neighboringTiles[CENTER]->mapTileType == MapTileType::LADDER ||
                                    neighboringTiles[CENTER]->mapTileType == MapTileType::LADDER_DECK) &&
                                   (neighboringTiles[DOWN_MIDDLE] == nullptr ||
                                    !neighboringTiles[DOWN_MIDDLE]->collidable);
            }

            if (climbing) {
                _y_speed = 1;
            }

            if ((!can_climb_rope && climbing && !on_top_of_climbing_space) || (!can_climb_ladder && climbing)) {
                jumping_timer = 0;
                climbing = false;
                started_climbing_rope = false;
                started_climbing_ladder = false;
            }

            hanging_on_tile_left = false;
            hanging_on_tile_right = false;
            if (_bottom_collision) {
                crawling = true;
                _pos_update_delta = main_dude_pos_update_delta_crawling;
                _max_x_speed = main_dude_max_x_crawling;
            }
        } else {
            crawling = false;
            if (!global::input_handler->r_bumper_held) {
                _max_x_speed = main_dude_max_x_speed_walking;
                _pos_update_delta = main_dude_pos_update_delta_walking_running;
            }
        }


    } else {
        crawling = false;
        if (!global::input_handler->r_bumper_held) {
            _max_x_speed = main_dude_max_x_speed_walking;
            _pos_update_delta = main_dude_pos_update_delta_walking_running;
        }
    }

    update_sprites_position();

}

void MainDude::boost_going_through_map_holes(MapTile **const t) {
    if (!_bottom_collision) {
        if ((t[TileOrientation::RIGHT_MIDDLE] == nullptr || !t[TileOrientation::RIGHT_MIDDLE]->collidable) &&
            (t[TileOrientation::RIGHT_UP] != nullptr && t[TileOrientation::RIGHT_DOWN] != nullptr)) {
            //if there's no collidable tile on right-mid, but there are on right-up and right-down,
            //add extra x-pos to ease going through a hole
            if (_x_speed > 0)
                _x += 2;
        }

        if ((t[TileOrientation::LEFT_MIDDLE] == nullptr || !t[TileOrientation::LEFT_MIDDLE]->collidable) &&
            (t[TileOrientation::LEFT_UP] != nullptr && t[TileOrientation::LEFT_DOWN] != nullptr)) {
            //same but for left side
            if (_x_speed < 0)
                _x -= 2;
        }
    }
}

void MainDude::update_creature_specific() {
    match_animation();
    reset_values_checked_every_frame();
    apply_blinking_on_damage();

    // update max speed
    if (crawling) {
        _max_x_speed = MAIN_DUDE_MAX_X_SPEED_CRAWLING;
        _max_y_speed = MAIN_DUDE_MAX_Y_SPEED;
    } else if (global::input_handler->r_bumper_held) {
        // running fast
        _max_x_speed = main_dude_max_x_speed_running;
        _max_y_speed = MAIN_DUDE_MAX_Y_SPEED;
    } else if (using_cape) {
        // falling, but with cape
        _max_x_speed = MAIN_DUDE_MAX_X_SPEED;
        _max_y_speed = MAIN_DUDE_MAX_Y_SPEED_USING_CAPE;
    } else {
        // default values
        _max_x_speed = MAIN_DUDE_MAX_X_SPEED;
        _max_y_speed = MAIN_DUDE_MAX_Y_SPEED;
    }

    if (crawling) {
        _pos_update_delta = main_dude_pos_update_delta_crawling;
        _max_x_speed = main_dude_max_x_crawling;
    } else {
        if (!global::input_handler->r_bumper_held)
            _pos_update_delta = main_dude_pos_update_delta_walking_running;
    }
    // "Update timers"


    pos_inc_timer += *global::timer;
    speed_inc_timer += *global::timer;
    hanging_timer += *global::timer;

    if (animation_frame_timer > 65) {

        animation_frame_timer = 0;

        if (!using_whip || (using_whip && animFrame < 5) || (started_climbing_rope && animFrame < 12) ||
            (exiting_level && animFrame < 16) || (started_climbing_ladder && animFrame < 6))
            animFrame++;

    }

    if (!global::input_handler->left_key_held && pushing_left) {
        pushing_left = false;
        pushing_timer = 0;
    }
    if (!global::input_handler->right_key_held && pushing_right) {
        pushing_right = false;
        pushing_timer = 0;
    }

    if ((_left_collision || _right_collision) && !crawling && !hanging_on_tile_left && !hanging_on_tile_right &&
        (global::input_handler->left_key_held || global::input_handler->right_key_held)) {
        pushing_timer += *global::timer;
        if (pushing_timer > MAIN_DUDE_PUSHING_TIME) {
            if (_left_collision) {
                pushing_right = true;
                pushing_timer = 0;
            } else {
                pushing_left = true;
                pushing_timer = 0;
            }
        }

    } else {
        pushing_timer = 0;
        pushing_left = false;
        pushing_right = false;
    }

    if (animFrame >= MAIN_DUDE_FRAMES_PER_ANIMATION && !crawling && !pushing_left && !pushing_right && !exiting_level)
        animFrame = 0;

    if (animFrame >= 9 && crawling)
        animFrame = 0;

    if ((pushing_left || pushing_right) && animFrame >= 7)
        animFrame = 0;


    if (!_bottom_collision && !hanging_on_tile_left && !hanging_on_tile_right && !climbing)
        jumping_timer += *global::timer;

    if (_bottom_collision && jumping_timer > MAIN_DUDE_STUN_FALLING_TIME) {


        if (global::hud->hearts > 0) {
            global::hud->hearts--;
            global::hud->draw_level_hud();
        }

        stunned = true;

        auto *f_left = new FallPoof(_x, _y);
        auto *f_right = new FallPoof(_x, _y);

        f_left->_x = _x - 4;
        f_right->_x = _x + MainDude::main_dude_physical_width - 6;

        f_left->_y = _y + 8;
        f_right->_y = _y + 8;

        global::creatures.push_back(f_left);
        global::creatures.push_back(f_right);

        if (global::hud->hearts == 0) {
            global::main_dude->set_dead();
        }

        mmEffect(SFX_XLAND);

        jumping_timer = 0;
    } else if (_bottom_collision && jumping_timer < MAIN_DUDE_STUN_FALLING_TIME) {
        jumping_timer = 0;
    }

    if (stunned)
        stunned_timer += *global::timer;
    if (stunned_timer > MAIN_DUDE_STUN_TIME) {
        stunned = false;
        stunned_timer = 0;
    }


    if (_x_speed != 0 || stunned || using_whip || (pushing_left || pushing_right) || (climbing && _y_speed != 0) ||
        exiting_level)
        animation_frame_timer += *global::timer;


    if (!_bottom_collision) {
        crawling = false;
        if (!global::input_handler->r_bumper_held) {
            _max_x_speed = main_dude_max_x_speed_walking;
            _pos_update_delta = main_dude_pos_update_delta_walking_running;
        }
    }

    time_since_last_jump += *global::timer;
    time_since_last_damage += *global::timer;

    // Map collisions

    if (_map_collisions_checked) {
        can_hang_on_tile(_neighboring_tiles);
        boost_going_through_map_holes(_neighboring_tiles);
        _map_collisions_checked = false;
    }

}

void MainDude::init_sprites() {

    delete_sprites();

    main_sprite_info = global::main_oam_manager->initSprite(gfx_spelunkerPal, gfx_spelunkerPalLen, nullptr,
                                                            _sprite_size, ObjSize::OBJSIZE_16, _spritesheet_type, true,
                                                            false,
                                                            LAYER_LEVEL::MIDDLE_TOP);


    sub_sprite_info = global::sub_oam_manager->initSprite(gfx_spelunkerPal, gfx_spelunkerPalLen, nullptr,
                                                          _sprite_size, ObjSize::OBJSIZE_16, _spritesheet_type, true,
                                                          false,
                                                          LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::set_vertical_flip(false, main_sprite_info, sub_sprite_info);
    sprite_utils::set_horizontal_flip(false, main_sprite_info, sub_sprite_info);

    update_sprites_position();
    match_animation();
}


void MainDude::reset_values_checked_every_frame() {
    can_climb_rope = false;
    can_climb_ladder = false;
}

#include <maxmod9.h>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include "../../GlobalsDeclarations.hpp"
#include "../items/Shotgun.hpp"
#include "../items/Mitt.hpp"
#include "../items/Glove.hpp"
#include "../items/Compass.hpp"
#include "../items/SpringShoes.hpp"
#include "../items/Rope.hpp"
#include "../../collisions/Collisions.hpp"
#include "../../tiles/LevelRenderingUtils.hpp"
#include "../../../build/soundbank.h"
#include "../../tiles/TileOrientation.hpp"
#include "../items/SpikeShoes.hpp"
#include "../items/Cape.hpp"
#include "../items/Jetpack.hpp"
#include "../../memory/SpriteUtils.hpp"
#include "../../../build/gfx_spelunker.h"
#include "../items/Pistol.hpp"
#include "MainDude.hpp"
#include "../../sound/SoundUtils.hpp"
#include "../../tiles/MapTile.hpp"
#include "../../tiles/Level.hpp"
#include "../../camera/Camera.hpp"
#include "../../memory/SpriteInfo.h"
#include "../items/Bomb.hpp"
#include "../../input/InputHandler.hpp"

void MainDude::throw_item() {

    //throw holding item
    //if holding bomb, arm it only

    if (_currently_held_item || _currently_held_creature || _currently_held_pickupable) {

        bool activated;
        ICollidable *held;
        if (_currently_held_item) {
            held = dynamic_cast<ICollidable *>(_currently_held_item);
            activated = _currently_held_item->_activated;
        } else if (_currently_held_creature) {
            held = dynamic_cast<ICollidable *>(_currently_held_creature);
            activated = _currently_held_creature->activated;
        } else {
            held = _currently_held_pickupable;
            activated = _currently_held_pickupable->_activated;
        }

        if (activated) {

            if (!global::input_handler->down_key_held) {

                if (carrying_mitt) {
                    if (sprite_state == Orientation::LEFT)
                        held->_x_speed = -6 - abs(_x_speed);
                    else
                        held->_x_speed = 6 + abs(_x_speed);

                } else {
                    if (sprite_state == Orientation::LEFT)
                        held->_x_speed = -4 - abs(_x_speed);
                    else
                        held->_x_speed = 4 + abs(_x_speed);
                }

            } else {

                if (sprite_state == Orientation::LEFT)
                    held->_x_speed = -0.04f;
                else
                    held->_x_speed = 0.04f;

            }


            if (_neighboring_tiles[TileOrientation::UP_MIDDLE]->exists &&
                _neighboring_tiles[TileOrientation::UP_MIDDLE]->collidable) {
                held->_y_speed = 0.0f;
                held->_x_speed *= 2;
            } else {

                if (global::input_handler->up_key_held)
                    held->_y_speed = -2.55 - abs(_y_speed);
                else {
                    held->_y_speed = -1;
                }

            }

            // it's not held anymore, so update flags
            if (_currently_held_item) {
                _currently_held_item->_hold_by_main_dude = false;
                _currently_held_item = nullptr;
            } else if (_currently_held_creature) {
                _currently_held_creature->hold_by_main_dude = false;
                _currently_held_creature = nullptr;
            } else {
                _currently_held_pickupable->_hold_by_main_dude = false;
                _currently_held_pickupable = nullptr;
            }


            holding_item = false;

            global::hud->disable_all_prompts();
            global::hud->draw_level_hud();

            mmEffect(SFX_XTHROW);

        } else {

            // activate item because it's not activated yet
            if (_currently_held_item)
                _currently_held_item->_activated = true;
            else if(_currently_held_creature)
                _currently_held_creature->activated = true;
            else
                _currently_held_pickupable->_activated = true;
        }

    }
}

void MainDude::take_out_bomb() {
    global::hud->bombs--;
    global::hud->draw_level_hud();
    Bomb *bomb = new Bomb(_x, _y);
    bomb->_hold_by_main_dude = true;
    global::items_to_add.push_back(bomb);
    holding_item = true;
}

void MainDude::throw_rope() {

    global::hud->ropes--;
    global::hud->draw_level_hud();

    u8 ROPE_PHYSICAL_WIDTH = 16;
    Rope *rope = new Rope((floor_div(_x + (0.5 * _physical_width), TILE_W) * TILE_W) + (ROPE_PHYSICAL_WIDTH * 0.5),
                          _y + 6);
    rope->_activated = true;
    rope->_y_speed = -4;
    global::items.push_back(rope);

}

void MainDude::spawn_carried_items() {

    if (carrying_spring_shoes) {
        auto *springShoes = new SpringShoes(HUD_ITEMS_ROW_X, global::hud->items_offset_y);
        springShoes->collected = true;
        springShoes->_bought = true;
        global::items_to_add.push_back(springShoes);
        global::hud->increment_offset_on_grabbed_item();
    }

    if (carrying_spike_shoes) {
        auto *spikeShoes = new SpikeShoes(HUD_ITEMS_ROW_X, global::hud->items_offset_y);
        spikeShoes->_render_in_hud = true;
        spikeShoes->_bought = true;
        global::items_to_add.push_back(spikeShoes);
        global::hud->increment_offset_on_grabbed_item();
    }

    if (carrying_compass) {
        auto *compass = new Compass(HUD_ITEMS_ROW_X, global::hud->items_offset_y);
        compass->collected = true;
        compass->_bought = true;
        global::items_to_add.push_back(compass);
        global::hud->increment_offset_on_grabbed_item();
    }
    if (carrying_glove) {
        auto *glove = new Glove(HUD_ITEMS_ROW_X, global::hud->items_offset_y);
        glove->_render_in_hud = true;
        glove->_bought = true;
        global::items.push_back(glove);
        global::hud->increment_offset_on_grabbed_item();
    }
    if (carrying_cape) {
        auto cape = new Cape(HUD_ITEMS_ROW_X, global::hud->items_offset_y);
        cape->_collected = true;
        cape->_bought = true;
        global::items_to_add.push_back(cape);
        global::hud->increment_offset_on_grabbed_item();
    }
    if (carrying_jetpack) {
        auto *jetpack = new Jetpack(HUD_ITEMS_ROW_X, global::hud->items_offset_y);
        jetpack->collected = true;
        jetpack->_bought = true;
        global::items_to_add.push_back(jetpack);
        global::hud->increment_offset_on_grabbed_item();
    }
    if (carrying_mitt) {
        auto mitt = new Mitt(HUD_ITEMS_ROW_X, global::hud->items_offset_y);
        mitt->collected = true;
        mitt->_bought = true;
        global::items_to_add.push_back(mitt);
        global::hud->increment_offset_on_grabbed_item();
    }
    if (carrying_shotgun) {
        holding_item = true;
        auto *shotgun = new Shotgun(_x, _y);
        shotgun->_bought = true;
        shotgun->_hold_by_main_dude = true;
        global::items_to_add.push_back(shotgun);
    }

    if (carrying_pistol) {
        holding_item = true;
        auto *pistol = new Pistol(_x, _y);
        pistol->_bought = true;
        pistol->_hold_by_main_dude = true;
        global::items.push_back(pistol);
    }
}


void MainDude::set_sprite_crawling() {

    if (sprite_state == Orientation::LEFT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size,
                                           animFrame + 24);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size,
                                           animFrame + 33);

}

void MainDude::set_sprite_hanging_on_tile() {

    if (hanging_on_tile_right)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 13);
    else if (hanging_on_tile_left)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 12);
}

void MainDude::set_sprite_whiping() {

    if (sprite_state == Orientation::LEFT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 56 + animFrame);
    else if (sprite_state == Orientation::RIGHT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 62 + animFrame);

}

void MainDude::set_sprite_pushing() {

    if (pushing_left)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 42 + animFrame);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 49 + animFrame);
}

void MainDude::set_sprite_stunned() {

    if (animFrame > 4)
        animFrame = 0;

    frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 18 + animFrame);
}

void MainDude::set_sprite_climbing() {

    if (started_climbing_rope) {

        if (animFrame >= 12)
            animFrame = 0;

        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 74 + animFrame);
    } else if (started_climbing_ladder) {

        if (animFrame >= 6)
            animFrame = 0;

        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 96 + animFrame);
    }

}

void MainDude::set_sprite_dead() {
    if (_bottom_collision)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 17);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 16);
}

void MainDude::set_sprite_walking_when_in_air() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, static_cast<u16>(sprite_state) * 6);
}

void MainDude::set_sprite_falling() {

    if (fabs(_x_speed) != 0)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size,
                                           animFrame +
                                           (static_cast<u16>(sprite_state) * MAIN_DUDE_FRAMES_PER_ANIMATION));
    else if (sprite_state == Orientation::LEFT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 14);
    else if (sprite_state == Orientation::RIGHT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 15);
}

void MainDude::set_sprite_exiting_level() {
    frameGfx = sprite_utils::get_frame((u8 *) gfx_spelunkerTiles, _sprite_size, 80 + animFrame);
}

void MainDude::apply_blinking_on_damage() {

    if (!global::game_state->levels_transition_screen) {

        if (time_since_last_damage < MAIN_DUDE_DAMAGE_PROTECTION_TIME) {
            sprite_utils::set_visibility(((int) time_since_last_damage % 100) >= 50, main_sprite_info, sub_sprite_info);
        } else
            sprite_utils::set_visibility(true, main_sprite_info, sub_sprite_info);

    }
}


void MainDude::can_hang_on_tile(MapTile **neighboringTiles) {

    if (_bottom_collision || (!_left_collision && !_right_collision))
        return;

    if ((neighboringTiles[UP_MIDDLE] != nullptr && neighboringTiles[UP_MIDDLE]->collidable) ||
        (neighboringTiles[DOWN_MIDDLE] != nullptr && neighboringTiles[DOWN_MIDDLE]->collidable))
        return;

    bool y_bound = false;
    bool x_bound = false;

    if (_right_collision && sprite_state == Orientation::LEFT) {

        if (!carrying_glove && (neighboringTiles[LEFT_UP] != nullptr && neighboringTiles[LEFT_UP]->collidable))
            return;

        x_bound = (this->_x <= (neighboringTiles[LEFT_MIDDLE]->x * 16) + 16 &&
                   (this->_x >= (neighboringTiles[LEFT_MIDDLE]->x * 16) + 12));
        y_bound = (this->_y > (neighboringTiles[LEFT_MIDDLE]->y * 16) - 2) &&
                  (this->_y < (neighboringTiles[LEFT_MIDDLE]->y * 16) + 8);
    } else if (_left_collision && sprite_state == Orientation::RIGHT) {

        if (!carrying_glove && (neighboringTiles[RIGHT_UP] != nullptr && neighboringTiles[RIGHT_UP]->collidable))
            return;

        y_bound = (this->_y > (neighboringTiles[RIGHT_MIDDLE]->y * 16) - 2) &&
                  (this->_y < (neighboringTiles[RIGHT_MIDDLE]->y * 16) + 8);
        x_bound = (this->_x <= (neighboringTiles[RIGHT_MIDDLE]->x * 16) - 12 &&
                   (this->_x >= (neighboringTiles[RIGHT_MIDDLE]->x * 16) - 16));
    }

    if ((y_bound && x_bound) && hanging_timer > MAIN_DUDE_MIN_HANGING_TIME) {

        if (_right_collision && neighboringTiles[LEFT_MIDDLE]->collidable) {
            this->_y = (neighboringTiles[LEFT_MIDDLE]->y * 16);
            hanging_on_tile_right = true;
            jumping_timer = 0;
            hanging_timer = 0;
            _y_speed = 0;
        }
        if (_left_collision && neighboringTiles[RIGHT_MIDDLE]->collidable) {
            jumping_timer = 0;
            hanging_on_tile_left = true;
            this->_y = (neighboringTiles[RIGHT_MIDDLE]->y * 16);
            hanging_timer = 0;
            _y_speed = 0;
        }
    }


    if (hanging_on_tile_right || hanging_on_tile_left) {
        using_cape = false;
        using_jetpack = false;
    }

    if (_upper_collision || _bottom_collision) {
        hanging_on_tile_left = false;
        hanging_on_tile_right = false;
        if (using_cape)
            jumping_timer = 0;
    }


}

void MainDude::apply_dmg(int dmg_to_apply) {
    if (dmg_to_apply == 4) {
        //fixme some enum that would indicate 'instant death, no matter for hp quantity' or a function kill_instantly
        //to differentiate (overloaded function)
        set_dead();
    }
}

void MainDude::match_animation() {

    if (exiting_level || (dead && global::input_handler->y_key_down)) {
        global::game_state->handle_changing_screens();
        set_sprite_exiting_level();
    } else if (using_whip) {
        set_sprite_whiping();
    } else if (dead) {
        set_sprite_dead();
    } else if (climbing) {
        set_sprite_climbing();
    } else if (stunned) {
        set_sprite_stunned();
    } else if (pushing_left || pushing_right) {
        set_sprite_pushing();
    } else if (hanging_on_tile_right || hanging_on_tile_left) {
        set_sprite_hanging_on_tile();
    } else if (crawling) {
        set_sprite_crawling();
    } else if (!_bottom_collision) {
        set_sprite_walking_when_in_air();
    } else {
        set_sprite_falling();
    }

    sprite_utils::update_frame(frameGfx, _sprite_size, main_sprite_info, sub_sprite_info);
}

void MainDude::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(main_sprite_info, static_cast<u16>(main_x), static_cast<u16>(main_y));
    sprite_utils::set_entry_xy(sub_sprite_info, static_cast<u16>(sub_x), static_cast<u16>(sub_y));
}

bool MainDude::can_apply_gravity() const {
    return !_bottom_collision && !climbing && !(hanging_on_tile_left || hanging_on_tile_right);
}

//TODO Enum dead cause?
void MainDude::set_dead() {
    time_since_last_damage = 0;
    global::hud->hearts = 0;
    global::hud->draw_level_hud();
    dead = true;
    _bouncing_factor_y = ICollidable::default_bouncing_factor_y;
    _y_speed = -MAIN_DUDE_JUMP_SPEED * 0.25;
    climbing = false;
    can_climb_rope = false;
    can_climb_ladder = false;
    hanging_on_tile_left = false;
    hanging_on_tile_right = false;
    pushing_left = false;
    pushing_right = false;
    consoleClear();
    sound::stop_cave_music();
    mmEffect(SFX_XDIE);
}

void MainDude::delete_sprites() {
    delete main_sprite_info;
    delete sub_sprite_info;
    main_sprite_info = nullptr;
    sub_sprite_info = nullptr;
}
