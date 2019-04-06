//
// Created by dbeef on 13.01.19.
//

#include "IPickupable.h"
#include "../../GlobalsDeclarations.hpp"
#include "../../collisions/Collisions.hpp"

//for opening chests and crates
bool IPickupable::check_if_can_be_opened() {
    if (!_activated &&
        Collisions::checkCollisionWithMainDudeWidthBoundary(_x, _y, _physical_width, _physical_height, 8) &&
        global::input_handler->up_key_held && global::input_handler->y_key_down) {

        _activated = true;
        global::input_handler->y_key_down = false;

        return true;
    } else
        return false;
}

//TODO Merge check_if_can_be_pickuped with set_pickuped_position
//check, if main dude can pickup this item (to hands, not to inventory)
void IPickupable::check_if_can_be_pickuped() {

    if (_hold_by_main_dude &&
        global::input_handler->y_key_down &&
        global::input_handler->down_key_held &&
        global::main_dude->_bottom_collision) {

        //leave item on ground

        _hold_by_main_dude = false;
        global::main_dude->holding_item = false;
        global::input_handler->y_key_down = false;
        _bottom_collision = false;
        global::main_dude->_currently_held_creature = nullptr;

    } else if (global::input_handler->y_key_down &&
               global::input_handler->down_key_held &&
               !global::main_dude->holding_item &&
               Collisions::checkCollisionWithMainDude(_x, _y, _physical_width, _physical_height)) {

        //pickup item from the ground

        global::main_dude->holding_item = true;
        global::main_dude->_currently_held_pickupable = this;
        _hold_by_main_dude = true;
        global::input_handler->y_key_down = false;

    }

};

//check, if main dude can pickup to the inventory (not to the hands)
bool IPickupable::check_if_can_be_equipped() {

    bool q = (global::input_handler->y_key_down && global::input_handler->down_key_held &&
              !global::main_dude->holding_item) &&
             Collisions::checkCollisionWithMainDude(_x, _y, _sprite_width, _sprite_height);

    if (q) {
        global::input_handler->y_key_down = false;
    }

    return q;

};