#pragma once

enum class InputEvent
{
    PAUSED,
    PAUSED_PRESSED,
    DEATH_REQUESTED,
    DEATH_REQUESTED_PRESSED,
    QUIT_REQUESTED,
    QUIT_REQUESTED_PRESSED,
    LEFT,
    LEFT_PRESSED,
    RIGHT,
    RIGHT_PRESSED,
    UP,
    UP_PRESSED,
    DOWN,
    DOWN_PRESSED,
    JUMPING,
    JUMPING_PRESSED,
    DUCKING,
    DUCKING_PRESSED,
    RUNNING_FAST,
    RUNNING_FAST_PRESSED,
    THROWING,
    THROWING_PRESSED,
    OUT_BOMB,
    OUT_BOMB_PRESSED,
    OUT_ROPE,
    OUT_ROPE_PRESSED,
    ACCEPT_TRANSACTION,
    ACCEPT_TRANSACTION_PRESSED,
};
