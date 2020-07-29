//
// Created by xdbeef on 04.03.18.
//

#ifndef SPELUNKYDS_LEVELGENERATOR_H
#define SPELUNKYDS_LEVELGENERATOR_H

#include "MapTile.hpp"
#include "SplashScreenType.hpp"
#include "RoomType.hpp"
#include "RenderEntity.hpp"

#include <vector>

namespace Consts
{
    const int ROOMS_WIDTH = 3;
    const int ROOMS_HEIGHT = 3;

    const int SPLASH_SCREEN_WIDTH = 20;
    const int SPLASH_SCREEN_HEIGHT = 12;

    const int ROOM_TILE_WIDTH_SPLASH_SCREEN = 20;
    const int ROOM_TILE_HEIGHT_SPLASH_SCREEN = 12;

    const int ROOM_TILE_WIDTH_GAME = 10;
    const int ROOM_TILE_HEIGHT_GAME = 10;

    const int LEVEL_HEIGHT_TILES = 32;
    const int LEVEL_WIDTH_TILES = 32;

    const int LINE_WIDTH = 64;
    const int OFFSET_X = 2; //Offset of 2 tiles, 8 px each
    const int OFFSET_Y = 128; //Offset of 128 tiles, 8px  each (2 lines, 64 each)
}

class TileBatch {

public:

    TileBatch();
    ~TileBatch();

    //I allocate these tiles on heap on game start, never delete them later. That helps memory fragmentation a lot.
    //Before, I allocated/deallocated them on every level and It caused crashes on malloc after some time (~40 levels).
    MapTile *map_tiles[Consts::LEVEL_WIDTH_TILES][Consts::LEVEL_HEIGHT_TILES]{};

    //holds information on what room type is at specific array index
    RoomType layout[Consts::ROOMS_WIDTH][Consts::ROOMS_HEIGHT]{};

    //holds information on specific variation of room type, that is given from 'layout' array
    //i.e, we have 6 possible 'closed' rooms declared in the closed_rooms.hpp,
    //so this array lets us know, that we have a 'closed' room number 3 (for example) at some place.
    int layout_room_ids[Consts::ROOMS_WIDTH][Consts::ROOMS_HEIGHT]{};

    //sets all tiles to !existing
    void clean_map_layout();

    void generate_new_level_layout();

    void generate_cave_background();

    void generate_frame();

    void initialise_tiles_from_room_layout();

    void initialise_tiles_from_splash_screen(SplashScreenType splashScreenType);

    void get_first_tile_of_given_type(MapTileType map_tile_type, MapTile *&out) const;

    void batch_vertices();

    void add_render_entity();

private:

    // Any encountered closed room will be turned into an altar.
    void place_an_altar();

    // Finds a closed room that is not blocked from either left or right side by other closed room,
    // and plants a shop there that is oriented to the not-blocked side.
    void place_a_shop();

    std::vector<Vertex> _mesh;
    std::vector<IndexType> _indices;
    RenderEntity _render_entity;
};

#endif //SPELUNKYDS_LEVELGENERATOR_H

