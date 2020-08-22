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

    static_assert(ROOMS_COUNT_WIDTH > 1, "Level generator will fail with current horizontal rooms count.");
    static_assert(ROOMS_COUNT_HEIGHT > 1, "Level generator will fail with current vertical rooms count.");

    const int SPLASH_SCREEN_WIDTH_TILES = 20;
    const int SPLASH_SCREEN_HEIGHT_TILES = 12;

    const int ROOM_WIDTH_TILES = 10;
    const int ROOM_HEIGHT_TILES = 10;

    const int LEVEL_HEIGHT_TILES = (ROOMS_COUNT_HEIGHT * ROOM_WIDTH_TILES) + 1 + 1; // 1 tile margin around the map
    const int LEVEL_WIDTH_TILES = (ROOMS_COUNT_WIDTH * ROOM_HEIGHT_TILES) + 1 + 1; // 1 tile margin around the map
}

class TileBatch {

public:

    TileBatch();

    ~TileBatch();

    void generate_new_level_layout();

    void generate_cave_background();

    void generate_frame();

    void initialise_tiles_from_room_layout();

    void initialise_tiles_from_splash_screen(SplashScreenType splashScreenType);

    void get_first_tile_of_given_type(MapTileType map_tile_type, MapTile *&out) const;

    void batch_vertices();

    void add_render_entity();

    void get_neighbouring_tiles(float x, float y, MapTile *out_neighboring_tiles[9]) const;

    MapTile *map_tiles[Consts::LEVEL_WIDTH_TILES][Consts::LEVEL_HEIGHT_TILES]{};

private:

    // Any encountered closed room will be turned into an altar.
    void place_an_altar();

    // Finds a closed room that is not blocked from either left or right side by other closed room,
    // and plants a shop there that is oriented to the not-blocked side.
    void place_a_shop();

    std::vector<Vertex> _mesh;
    std::vector<IndexType> _indices;
    RenderEntity _render_entity;

    // TODO: Interface to query XY (in tiles) for loot type.
    // TODO: Level class will handle populating.

    RoomType _layout[Consts::ROOMS_COUNT_WIDTH][Consts::ROOMS_COUNT_HEIGHT]{};
    int _layout_room_ids[Consts::ROOMS_COUNT_WIDTH][Consts::ROOMS_COUNT_HEIGHT]{};
};

#endif //SPELUNKYDS_LEVELGENERATOR_H
