#pragma once

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
