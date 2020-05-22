//
// Created by dbeef on 5/22/20.
//

#ifndef SPELUNKY_PSP_GAMELOOPBASESTATE_HPP
#define SPELUNKY_PSP_GAMELOOPBASESTATE_HPP

#pragma once

#include <cstdint>

class GameLoop;

class GameLoopBaseState
{
public:
    ~GameLoopBaseState() = default;
    virtual GameLoopBaseState* update(GameLoop&, uint32_t delta_time_ms) = 0;
    virtual void enter(GameLoop&) = 0;
    virtual void exit(GameLoop&) = 0;
};


#endif //SPELUNKY_PSP_GAMELOOPBASESTATE_HPP
