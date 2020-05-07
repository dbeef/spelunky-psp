//
// Created by xdbeef on 04.03.18.
//

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <Level.hpp>

#include "Renderer.hpp"
#include "Level.hpp"
#include "SplashScreenType.hpp"
#include "EntranceRooms.hpp"
#include "ExitRooms.hpp"
#include "ClosedRooms.hpp"
#include "LeftRightRooms.hpp"
#include "LeftRightDownRooms.hpp"
#include "LeftRightUpRooms.hpp"
#include "SplashScreenRooms.hpp"
#include "AltarRoom.hpp"
#include "RoomType.hpp"
#include "ShopRooms.hpp"
#include "Direction.hpp"

using namespace Consts;

void Level::generate_frame() {

    //upper row
    for (int a = 0; a < MAP_GAME_HEIGHT_TILES; a++) {
//        delete (map_tiles[a][0]);
//        auto *t = new MapTile();
        map_tiles[a][0]->match_tile(MapTileType::CAVE_REGULAR);
        map_tiles[a][0]->destroyable = false; //make it non-destroyable explicitly
        map_tiles[a][0]->x = a;
        map_tiles[a][0]->y = 0;
        map_tiles[a][0]->exists = true;
    }

    //bottom row
    for (int a = 0; a < MAP_GAME_WIDTH_TILES; a++) {
//        delete (map_tiles[31][a]);
//        auto *t = new MapTile();
        map_tiles[31][a]->match_tile(MapTileType::CAVE_REGULAR);
        map_tiles[31][a]->destroyable = false; //make it non-destroyable explicitly
        map_tiles[31][a]->x = 31;
        map_tiles[31][a]->y = a;
        map_tiles[31][a]->exists = true;
    }

    //right row
    for (int a = 0; a < MAP_GAME_HEIGHT_TILES; a++) {
//        delete (map_tiles[31][a]);
//        auto *t = new MapTile();
        map_tiles[a][31]->match_tile(MapTileType::CAVE_REGULAR);
        map_tiles[a][31]->destroyable = false; //make it non-destroyable explicitly
        map_tiles[a][31]->x = a;
        map_tiles[a][31]->y = 31;
        map_tiles[a][31]->exists = true;
    }

    //left row
    for (int a = 0; a < MAP_GAME_WIDTH_TILES; a++) {
//        delete (map_tiles[0][a]);
//        auto *t = new MapTile();
        map_tiles[0][a]->match_tile(MapTileType::CAVE_REGULAR);
        map_tiles[0][a]->destroyable = false; //make it non-destroyable explicitly
        map_tiles[0][a]->x = 0;
        map_tiles[0][a]->y = a;
        map_tiles[0][a]->exists = true;
    }
}

/**
 * Initialises every MapTile in the map_tiles[][] array with its position on the map,
 * so it could be allowed to call LevelGenerator::tiles_to_map.
 * It utilizes given splash screen type, to know if any tile should be planted on the place it iterates,
 * and what MapTileType it should have.
 */
void Level::initialise_tiles_from_splash_screen(SplashScreenType splash_type) {

    int tab[SPLASH_SCREEN_HEIGHT][SPLASH_SCREEN_WIDTH];
    bool offset_on_upper_screen = false;

    if (splash_type == ON_LEVEL_DONE_UPPER || splash_type == SCORES_UPPER || splash_type == MAIN_MENU_UPPER) {
        offset_on_upper_screen = true;

        if (splash_type == MAIN_MENU_UPPER)
            memcpy(tab, main_menu_upper, sizeof(main_menu_upper));
        else if (splash_type == ON_LEVEL_DONE_UPPER)
            memcpy(tab, on_level_done_upper, sizeof(on_level_done_upper));
        else if (splash_type == SCORES_UPPER)
            memcpy(tab, scores_upper, sizeof(scores_upper));
    }

    if (splash_type == ON_LEVEL_DONE_LOWER || splash_type == SCORES_LOWER || splash_type == MAIN_MENU_LOWER) {

        if (splash_type == ON_LEVEL_DONE_LOWER)
            memcpy(tab, on_level_done_lower, sizeof(on_level_done_lower));
        else if (splash_type == MAIN_MENU_LOWER)
            memcpy(tab, main_menu_lower, sizeof(main_menu_lower));
        else if (splash_type == SCORES_LOWER)
            memcpy(tab, scores_lower, sizeof(scores_lower));
    }

    //Now we initialise every tile in the splash screen and give it a map_index, which describes its location
    for (int tab_y = 0; tab_y < SPLASH_SCREEN_HEIGHT; tab_y++) {
        for (int tab_x = 0; tab_x < SPLASH_SCREEN_WIDTH; tab_x++) {

//            if (tab[tab_y][tab_x] != 0) {

            //offset to the position in current room
            int room_offset =
                    static_cast<int>(
                            2 * ROOM_TILE_HEIGHT_SPLASH_SCREEN *
                            LINE_WIDTH * ((ROOMS_Y - offset_on_upper_screen) - 1) - 4 * OFFSET_Y);
            //pos x and y in pixels of the tile in the current room
            int pos_x = static_cast<int>((tab_x * 2) / 2);
            //NDS engine has different coordinate system than our room layout map,
            //so we invert the Y axis by ((ROOMS_Y - offset_on_upper_screen) - 1)
            int pos_y = static_cast<int>(
                    tab_y + ROOM_TILE_HEIGHT_SPLASH_SCREEN * ((ROOMS_Y - offset_on_upper_screen) - 1) - 4);

            map_tiles[pos_x][pos_y]->match_tile(static_cast<MapTileType>(tab[tab_y][tab_x]));
            room_offset + (tab_x * 2) + (LINE_WIDTH + (tab_y * LINE_WIDTH * 2)) + 1;
            map_tiles[pos_x][pos_y]->x = pos_x;
            map_tiles[pos_x][pos_y]->y = pos_y;

            if (tab[tab_y][tab_x] != 0)
                map_tiles[pos_x][pos_y]->exists = true;
//            }
        }
    }
}

/**
 * Initialises every MapTile in the map_tiles[][] array with its position on the map,
 * so it could be allowed to call LevelGenerator::tiles_to_map.
 * It utilizes current room layout, to know if any tile should be planted on the place it iterates,
 * and what MapTileType it should have.
 */

void Level::initialise_tiles_from_room_layout() {

    //a room, 10x10 tiles
    int tab[10][10];
    int r;

    //iterate through every room we have
    for (int room_y = ROOMS_Y - 1; room_y >= 0; room_y--) {
        for (int room_x = 0; room_x < ROOMS_X; room_x++) {

            //basing on the room type, randomly select a variation of this room
            //and copy it to the temporary tab[10][10] array
            int room_type = layout[room_x][room_y];
            r = rand() % 6;
            layout_room_ids[room_x][room_y] = r; //-1 if completely disabling NPC's in this room

            //copying specific room variation
            switch (room_type) {
                case R_CLOSED:
                    memcpy(tab, closed_rooms[r], sizeof(closed_rooms[r]));
                    break;
                case R_LEFT_RIGHT:
                    memcpy(tab, left_right_rooms[r], sizeof(left_right_rooms[r]));
                    break;
                case R_LEFT_RIGHT_DOWN:
                    memcpy(tab, left_right_down_rooms[r], sizeof(left_right_down_rooms[r]));
                    break;
                case R_LEFT_RIGHT_UP:
                    memcpy(tab, left_right_up_rooms[r], sizeof(left_right_up_rooms[r]));
                    break;
                case R_ENTRANCE:
                    memcpy(tab, entrance_room[r], sizeof(entrance_room[r]));
                    break;
                case R_EXIT:
                    memcpy(tab, exit_room[r], sizeof(exit_room[r]));
                    break;
                case R_SHOP_LEFT:
                    memcpy(tab, shops[0], sizeof(shops[0]));
                    break;
                case R_SHOP_RIGHT:
                    memcpy(tab, shops[1], sizeof(shops[1]));
                    break;
                case R_SHOP_LEFT_MUGSHOT:
                    memcpy(tab, shops_mugshots[0], sizeof(shops_mugshots[0]));
                    break;
                case R_SHOP_RIGHT_MUGSHOT:
                    memcpy(tab, shops_mugshots[1], sizeof(shops_mugshots[1]));
                    break;
                case R_ALTAR:
                    memcpy(tab, altar_room[0], sizeof(altar_room[1]));
                    break;
                default:
                    break;
            }

            //Now we initialise every tile in map and give it a map_index, which describes its location
            for (int tab_y = 0; tab_y < ROOM_TILE_HEIGHT_GAME; tab_y++) {
                for (int tab_x = 0; tab_x < ROOM_TILE_WIDTH_GAME; tab_x++) {


                    //pos x and y in pixels of the tile in the current room
                    int pos_x = static_cast<int>((OFFSET_X + tab_x * 2 + 2 * ROOM_TILE_WIDTH_GAME * room_x) / 2);
                    //NDS engine has different coordinate system than our room layout map,
                    //so we invert the Y axis by ((ROOMS_Y - room_y) - 1))
                    int pos_y = static_cast<int>(
                            (OFFSET_X + tab_y * 2 + 2 * ROOM_TILE_HEIGHT_GAME * ((ROOMS_Y - room_y) - 1)) / 2);

                    map_tiles[pos_x][pos_y]->match_tile(static_cast<MapTileType>(tab[tab_y][tab_x]));

                    map_tiles[pos_x][pos_y]->x = pos_x;
                    map_tiles[pos_x][pos_y]->y = pos_y;
                    if (tab[tab_y][tab_x] != 0)
                        map_tiles[pos_x][pos_y]->exists = true;

                }
            }
        }
    }
}

/**
 * Sets given MapTile to the first tile on map that matches given MapTileType.
 * Searching begins on the upper-left corner of the map and goes right-down.
 * https://stackoverflow.com/questions/416162/assignment-inside-function-that-is-passed-as-pointer
 * @param mapTileType
 * @param m
 */
void Level::get_first_tile_of_given_type(MapTileType mapTileType, MapTile *&m) {
    for (int a = 0; a < MAP_GAME_WIDTH_TILES; a++) {
        for (int b = 0; b < MAP_GAME_HEIGHT_TILES; b++) {
            if (map_tiles[a][b]->exists && map_tiles[a][b]->mapTileType == mapTileType) {
                m = map_tiles[a][b];
                break;
            }
        }
    }
}

void Level::clean_map_layout() {
    //clean current layout
    for (int x = 0; x < 32; x++)
        for (int y = 0; y < 32; y++) {
            map_tiles[x][y]->exists = false;
            map_tiles[x][y]->destroyable = true; //tiles are destroyable by default
        }
}

Level::Level()
{
    for (int a = 0; a < 32; a++)
        for (int b = 0; b < 32; b++) {
            map_tiles[a][b] = new MapTile();
            map_tiles[a][b]->x = a;
            map_tiles[a][b]->y = b;
        }
}

Level::~Level()
{
    for (int a = 0; a < 32; a++)
        for (int b = 0; b < 32; b++) {
            delete map_tiles[a][b];
        }
}

void Level::batch_vertices()
{
    const auto &camera = Camera::instance();

    _render_batch.xyz.clear();
    _render_batch.uv.clear();
    _render_batch.indices.clear();
    _render_batch.tile_counter = 0;

    // FIXME: Rewrite level generator for more sane convention of storing tiles.
    // iterating from left-lower corner of the room to the right-upper (spelunky-ds convention)
    for (int x = 0; x < Consts::MAP_GAME_WIDTH_TILES; x++)
    {
        for (int y = 0; y < Consts::MAP_GAME_HEIGHT_TILES; y++)
        {

            MapTile *t = map_tiles[x][y];

            // FIXME: Remove x/y fields from MapTile, as they are redundant.
            assert(map_tiles[x][y]->x == x);
            assert(map_tiles[x][y]->y == y);

            auto tile_type = static_cast<int>(t->mapTileType);
            const auto &tile = TextureBank::instance().get_region(TextureType::CAVE_LEVEL_TILES, tile_type);

            tile.push_positions(_render_batch.xyz, x, y);
            tile.push_indices(_render_batch.indices, _render_batch.tile_counter);
            tile.push_uvs(_render_batch.uv);
            _render_batch.tile_counter++;
        }
    }

    // This could be done in the loop before, but this way is more readable, and does not affect performance much
    // as batching vertices is not done very often.
    assert(_render_batch.xyz.size() == _render_batch.uv.size());
    for (std::size_t index = 0; index < _render_batch.xyz.size(); index += 2)
    {
        Vertex vertex{};
        vertex.x = _render_batch.xyz[index];
        vertex.y = _render_batch.xyz[index + 1];
        vertex.u = _render_batch.uv[index];
        vertex.v = _render_batch.uv[index + 1];
        _render_batch.merged.push_back(vertex);
    }
}

void Level::add_render_entity()
{
    auto& renderer = Renderer::instance();
    _render_entity.vertices = _render_batch.merged.data();
    _render_entity.indices = _render_batch.indices.data();
    _render_entity.indices_count = _render_batch.indices.size();
    _render_entity.texture = TextureBank::instance().get_texture(TextureType::CAVE_LEVEL_TILES);
    _render_entity.id = renderer.add_entity(_render_entity);
}
