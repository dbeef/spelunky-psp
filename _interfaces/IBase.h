//
// Created by xdbeef on 29.12.18.
//

#ifndef SPELUNKYDS_IBASE_H
#define SPELUNKYDS_IBASE_H

#include <stdio.h>
#include <nds/interrupts.h>

// Base delivers basic functionality needed by every creature/item/decoration etc.
class IBase {

public:

    IBase() = default;

    virtual ~IBase() = default;

    inline void set_xy(int const &x, int const &y) {
        _x = x;
        _y = y;
    }

    int _x;
    int _y;
    bool _ready_to_dispose{};

    void set_pickuped_position_not_checking(int pickup_offset_x_left, int pickup_offset_x_right, int pickup_offset_y);

};


#endif //SPELUNKYDS_IBASE_H
