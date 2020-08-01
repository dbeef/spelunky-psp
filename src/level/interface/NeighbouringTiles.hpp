#pragma once

#include <cstdint>

enum class NeighbouringTiles : std::uint16_t
{
    LEFT_MIDDLE = 0,
    RIGHT_MIDDLE = 1,
    UP_MIDDLE = 2,
    DOWN_MIDDLE = 3,
    CENTER = 4,
    LEFT_UP = 5,
    RIGHT_UP = 6,
    LEFT_DOWN = 7,
    RIGHT_DOWN = 8,
    _SIZE = 9
};
