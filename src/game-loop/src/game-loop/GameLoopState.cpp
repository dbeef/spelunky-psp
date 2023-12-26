#include "game-loop/GameLoopState.hpp"
#include <cassert>

const char *to_string(GameLoopState game_loop_state) {
#define TO_STRING(x) case GameLoopState::x: return #x;
    switch (game_loop_state) {
        TO_STRING(MAIN_MENU);
        TO_STRING(PLAYING);
        TO_STRING(STARTED);
        TO_STRING(LEVEL_SUMMARY);
        TO_STRING(SCORES);
        TO_STRING(SANDBOX);
        TO_STRING(CURRENT);
        TO_STRING(_SIZE);
    }
    assert(false);
    return "Failed to match passed GameLoopState";
}