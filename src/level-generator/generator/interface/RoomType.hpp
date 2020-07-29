//
// Created by xdbeef on 02.05.18.
//

#ifndef SPELUNKYDS_ROOMTYPE_H
#define SPELUNKYDS_ROOMTYPE_H

#include <cstdint>

enum class RoomType : std::uint16_t
{
    CLOSED,
    LEFT_RIGHT,
    LEFT_RIGHT_DOWN,
    LEFT_RIGHT_UP,
    EXIT,
    ENTRANCE,
    SHOP_LEFT,
    SHOP_RIGHT,
    SHOP_LEFT_MUGSHOT,
    SHOP_RIGHT_MUGSHOT,
    ALTAR,
};

#endif //SPELUNKYDS_ROOMTYPE_H
