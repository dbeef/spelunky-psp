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
    const int ROOMS_COUNT_WIDTH = 3;
    const int ROOMS_COUNT_HEIGHT = 3;

    const int SPLASH_SCREEN_WIDTH_TILES = 20;
    const int SPLASH_SCREEN_HEIGHT_TILES = 12;

    const int ROOM_WIDTH_TILES = 10;
    const int ROOM_HEIGHT_TILES = 10;

    const int LEVEL_HEIGHT_TILES = (ROOMS_COUNT_WIDTH * ROOM_WIDTH_TILES) + 1 + 1; // 1 tile margin around the map
    const int LEVEL_WIDTH_TILES = (ROOMS_COUNT_HEIGHT * ROOM_HEIGHT_TILES) + 1 + 1; // 1 tile margin around the map
}

// TODO: Dimensions should be parametrized, consts should go upwards in the architectural scope.

class TileBatch {

public:

    TileBatch();
    ~TileBatch();

    //I allocate these tiles on heap on game start, never delete them later. That helps memory fragmentation a lot.
    //Before, I allocated/deallocated them on every level and It caused crashes on malloc after some time (~40 levels).
    MapTile *map_tiles[Consts::LEVEL_WIDTH_TILES][Consts::LEVEL_HEIGHT_TILES]{};

    //holds information on what room type is at specific array index
    RoomType layout[Consts::ROOMS_COUNT_WIDTH][Consts::ROOMS_COUNT_HEIGHT]{};

    //holds information on specific variation of room type, that is given from 'layout' array
    //i.e, we have 6 possible 'closed' rooms declared in the closed_rooms.hpp,
    //so this array lets us know, that we have a 'closed' room number 3 (for example) at some place.
    int layout_room_ids[Consts::ROOMS_COUNT_WIDTH][Consts::ROOMS_COUNT_HEIGHT]{};

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

