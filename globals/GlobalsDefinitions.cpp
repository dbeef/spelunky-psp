//
// Created by xdbeef on 02.09.18.
//

#include "GlobalsDeclarations.hpp"

//https://softwareengineering.stackexchange.com/questions/245236/is-heap-fragmentation-a-problem-in-bare-metal-arm-with-g
void global::init_globals() {

    input_handler = new InputHandler();
    camera = new Camera();
    main_dude = nullptr;
    current_level = new Level();
    current_level->init_map_tiles();
    main_oam_manager = new OAMManager();
    sub_oam_manager = new OAMManager();
    hud = new Hud();
    game_state = new GameState();

    //some of the moving objects aren't rendered as entities, like the ArrowTrap, which is render as a map tile,
    //so the the list is bigger, though the maximum number of entities that can be rendered on NDS is 128
    global::creatures.reserve(64);
    //some of the entities in this list may just wait to be disposed, that's why this size is slightly bigger that 128
    global::sprite_infos.reserve(128);
    //I assume there won't be a situation when we'll add more than 64 objects in a single frame
    global::creatures_to_add.reserve(64);

    global::decorations.reserve(64);
    global::decorations_to_add.reserve(64);

    global::treasures.reserve(8);
    global::treasures_to_add.reserve(8);

}
