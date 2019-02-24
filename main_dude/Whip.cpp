//
// Created by xdbeef on 07.05.18.
//

#include "Whip.hpp"
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../../GlobalsDeclarations.hpp"
#include "../../memory/SpriteUtils.hpp"

void Whip::update_creature_specific() {

    if (global::main_dude->using_whip) {
        update_sprites_position();

        _whiping_timer += *global::timer;
        if (_whiping_timer > 420) {
            _whiping_timer = 0;
            global::main_dude->using_whip = false;
            hide();
        }

    } else
        hide();

    _x = global::main_dude->_x;
    _y = global::main_dude->_y - 1;

    match_animation();

}

void Whip::update_sprites_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(_main_sprite_info, static_cast<u16>(main_x), static_cast<u16>(main_y));
    sprite_utils::set_entry_xy(_sub_sprite_info, static_cast<u16>(sub_x), static_cast<u16>(sub_y));
}

void Whip::init_sprites() {

    delete_sprites();

    _main_sprite_info = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                             gfx_spike_collectibles_flamePalLen, nullptr,
                                                             _sprite_size, ObjSize::OBJSIZE_16, WHIP,
                                                             true, false, LAYER_LEVEL::MIDDLE_TOP);

    _sub_sprite_info = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                           gfx_spike_collectibles_flamePalLen, nullptr,
                                                           _sprite_size, ObjSize::OBJSIZE_16, WHIP, true, false,
                                                           LAYER_LEVEL::MIDDLE_TOP);
    sprite_utils::set_vertical_flip(false, _main_sprite_info, _sub_sprite_info);
    hide();
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Whip::assign_pre_whip_sprite() {
    _frame_gfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size, 41);
}

//!> after calling this function, call sprite_utils::update_frame to update OAM with current frameGfx
void Whip::assign_whip_sprite() {
    _frame_gfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, _sprite_size, 40);
}

void Whip::hide() {
    sprite_utils::set_visibility(false, _main_sprite_info, _sub_sprite_info);
}

void Whip::delete_sprites() {
    delete _main_sprite_info;
    delete _sub_sprite_info;
    _main_sprite_info = nullptr;
    _sub_sprite_info = nullptr;
}

void Whip::match_animation() {

    if (_whiping_timer > 100 && _whiping_timer < 180) {

        //whiping, phase 1 - whip is behind main dude
        assign_pre_whip_sprite();
        sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);

        if (global::main_dude->sprite_state == Orientation::LEFT) {
            _x += 8;
            sprite_utils::set_horizontal_flip(true, _main_sprite_info, _sub_sprite_info);
        } else {
            _x -= 8;
            sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
        }

    } else if (_whiping_timer >= 220) {

        //whipping, phase 2 - whip is in front of main dude
        assign_whip_sprite();
        sprite_utils::set_visibility(true, _main_sprite_info, _sub_sprite_info);

        if (global::main_dude->sprite_state == Orientation::LEFT) {
            _x -= 16;
            sprite_utils::set_horizontal_flip(false, _main_sprite_info, _sub_sprite_info);
        } else {
            _x += 16;
            sprite_utils::set_horizontal_flip(true, _main_sprite_info, _sub_sprite_info);
        }

    } else
        hide();

    sprite_utils::update_frame(_frame_gfx, _sprite_size, _main_sprite_info, _sub_sprite_info);

}
