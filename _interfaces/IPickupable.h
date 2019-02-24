//
// Created by dbeef on 13.01.19.
//

#ifndef SPELUNKYDS_IPICKUPABLE_H
#define SPELUNKYDS_IPICKUPABLE_H

#include "IRenderable.h"
#include "ICollidable.h"

class IPickupable : public ICollidable, public IRenderable {

public:

    IPickupable() = delete;

    IPickupable(int x, int y,
                u16 sprite_width, u16 sprite_height, SpritesheetType spritesheet_type,
                u16 physical_width, u16 physical_height) :
            ICollidable(physical_width, physical_height),
            IRenderable(sprite_width, sprite_height, spritesheet_type) {
        // do nothing
    }

    bool check_if_can_be_opened();

    void check_if_can_be_pickuped();

    bool check_if_can_be_equipped();

    //

    bool _hold_by_main_dude{};

    bool _activated{};

};


#endif //SPELUNKYDS_IPICKUPABLE_H
