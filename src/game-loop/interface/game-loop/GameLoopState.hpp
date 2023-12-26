#pragma once

#include <cstdint>

using GameLoopState_t = std::uint16_t;
enum class GameLoopState : GameLoopState_t
{
    MAIN_MENU = 0,
    PLAYING,
    STARTED,
    LEVEL_SUMMARY,
    SCORES,
    SANDBOX,
    CURRENT,
    _SIZE
};

const char* to_string(GameLoopState);