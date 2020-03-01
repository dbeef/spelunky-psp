//
// Created by dbeef on 2/5/20.
//

#ifndef RESOURCE_COMPILER_GAMESTATE_HPP
#define RESOURCE_COMPILER_GAMESTATE_HPP

enum class GameState
{
    ENTERED_GAME, // Loads assets and enters MAIN_MENU state.
    MAIN_MENU, // Generates main menu level, creates spelunker and delegates input to the player.
};

#endif //RESOURCE_COMPILER_GAMESTATE_HPP
