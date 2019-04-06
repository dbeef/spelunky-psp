//
// Created by xdbeef on 27.05.18.
//

#ifndef SPELUNKYDS_GAME_STATE_H
#define SPELUNKYDS_GAME_STATE_H

#include "../../tiles/MapTileType.hpp"

class GameState {
public:
    bool just_started_game = true;
    bool exiting_game{};
    bool robbed_killed_shopkeeper{};
    bool bombed{};
    bool splash_screen{};
    bool levels_transition_screen{};
    bool scores_screen{};
    bool in_main_menu{};
    bool spawned_smooch{};
    bool smooching{};
    int damsels_rescued_this_level{};
    int smooch_timer{};
    int brightness_level = 16;
    int change_brightness_timer{};

    void start_new_game();

    void start_main_menu();

    void start_scores();

    void start_level_transition_screen();

    void start_next_level();

    void handle_changing_screens();

    void set_position_to(MapTileType t);

    void handle_transition_screen_smooch();

    void reset_main_dude();
};

#endif //SPELUNKYDS_GAME_STATE_H
