//
// Created by xdbeef on 03.09.18.
//

#include <cstdarg>
#include <cstdio>
#include "SpriteUtils.hpp"

namespace sprite_utils {

    void set_entry_xy(SpriteInfo *i, u16 x, u16 y) {
        i->entry->x = x;
        i->entry->y = y;
    }

    u8 *get_frame(u8 *tileset, int frame_size, int frame_offset) {
        return tileset + ((frame_size * frame_offset) / 2);
    }


}