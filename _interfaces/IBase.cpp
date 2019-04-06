//
// Created by xdbeef on 29.12.18.
//

#include "IBase.h"
#include "../../GlobalsDeclarations.hpp"

void IBase::set_pickuped_position_not_checking(
        int pickup_offset_x_left,
        int pickup_offset_x_right,
        int pickup_offset_y) {

    _y = global::main_dude->_y + pickup_offset_y;

    if (global::main_dude->sprite_state == Orientation::LEFT) {
        _x = global::main_dude->_x - pickup_offset_x_left;
    } else
        _x = global::main_dude->_x + pickup_offset_x_right;

}
