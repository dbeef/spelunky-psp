//
// Created by xdbeef on 04.03.18.
//

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <algorithm>

#include "LevelGenerator.hpp"
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

/**
 * Used in the process of placing rooms.
 * !\relates generate_new_rooms
 */
void obtain_new_direction(int curr_x, Direction &direction) {
    if (curr_x == 0)
        //we're on the left side of the map, so go right
        direction = Direction::RIGHT;
    else if (curr_x == 2)
        //we're on the right side of the map, so go left
        direction = Direction::LEFT;
    else
        //we're in the middle, so make a guess where should we gow now
        direction = static_cast<Direction>(rand() % 2); //left or right
}

void Level::generate_new_level_layout() 
{
    auto& level = LevelGenerator::instance().getLevel();
    
    //clean current layout
    level.clean_map_layout();
    for (auto &room_type : level.layout)
        for (RoomType &b : room_type)
            //not visited rooms are of CLOSED type by default
            b = RoomType::R_CLOSED;

    //generate new seed for the random number generator
//    srand(timerElapsed(1));

    //set starting position to the random room in the most upper row
    int curr_x = rand() % 3;
    int curr_y = ROOMS_Y - 1;
    //direction represents where the generator will go in the next loop iteration
    Direction direction;
    obtain_new_direction(curr_x, direction);

    bool exit_placed = false;

    //set the starting room as an entrance room
    level.layout[curr_x][curr_y] = RoomType::R_ENTRANCE;

    //while we're on the very bottom floor or higher, do
    while (curr_y >= 0) {

        if (direction == Direction::LEFT || direction == Direction::RIGHT) {

            if ((direction == Direction::LEFT && curr_x == 0) || (direction == Direction::RIGHT && curr_x == 2)) {
                //our direction is to go left, but we're already on the left side of the map, so go down
                direction = Direction::DOWN;
            } else {

                if (direction == Direction::LEFT)
                    //our direction is to go left, and we're not on the left side of the map yet
                    curr_x--;
                else
                    //same, if right side
                    curr_x++;

                if (curr_y == 0 && !exit_placed && rand() % 2 == 0) {
                    //we're on the most bottom floor, we didn't plant an exit yet and we've guessed that's the place
                    exit_placed = true;
                    level.layout[curr_x][curr_y] = RoomType::R_EXIT;
                } else
                    level.layout[curr_x][curr_y] = RoomType::R_LEFT_RIGHT;

                if (rand() % 3 == 2)
                    //random chance that we change our direction to go down in the next iteration
                    direction = Direction::DOWN;
            }

        } else if (direction == Direction::DOWN) {

            if (curr_y > 0) {

                level.layout[curr_x][curr_y] = RoomType::R_LEFT_RIGHT_DOWN;
                curr_y--;
                level.layout[curr_x][curr_y] = RoomType::R_LEFT_RIGHT_UP;

                if (curr_y == 0 && !exit_placed && rand() % 2 == 0) {
                    //if we're on the very bottom floor, no exit planted yet and a guess tells us so, place an exit
                    exit_placed = true;
                    level.layout[curr_x][curr_y] = RoomType::R_EXIT;
                }

                obtain_new_direction(curr_x, direction);
            } else {

                if (!exit_placed)
                    //we're on the very bottom floor, didn't plant an exit yet and we're
                    //done with iterating through map, so plant an exit
                    level.layout[curr_x][curr_y] = RoomType::R_EXIT;

                break;
            }

        }
    }

    //TODO more post-generation effects, i.e if there's a column of '0' type rooms, then make a snake well
    place_an_altar();
    place_a_shop();
}

void Level::place_an_altar() {
    auto& level = LevelGenerator::instance().getLevel();
    for (int a = 0; a < ROOMS_X; a++) {
        for (int b = 0; b < ROOMS_Y; b++) {
            if (level.layout[a][b] == RoomType::R_CLOSED) {
                level.layout[a][b] = RoomType::R_ALTAR;
                return;
            }
        }
    }
}


/**
 * Finds a closed room that is not blocked from left or right side by other closed room,
 * and plants a shop there that is oriented to the not-blocked side.
 * !\relates generate_new_rooms
 */
void Level::place_a_shop() {
    auto& level = LevelGenerator::instance().getLevel();

    for (int a = 0; a < ROOMS_X; a++) {
        for (int b = 0; b < ROOMS_Y; b++) {
            if (level.layout[a][b] == RoomType::R_CLOSED) {
                if (a == 0) {
                    if (level.layout[a + 1][b] != RoomType::R_CLOSED) {
//                        if (GameState::instance().robbed_or_killed_shopkeeper)
                        if (false)
                            level.layout[a][b] = RoomType::R_SHOP_RIGHT_MUGSHOT;
                        else
                            level.layout[a][b] = RoomType::R_SHOP_RIGHT;
                        return;
                    }
                } else if (a == 2) {
                    if (level.layout[a - 1][b] != RoomType::R_CLOSED) {
//                        if (GameState::instance().robbed_or_killed_shopkeeper)
                        if (false)
                            level.layout[a][b] = RoomType::R_SHOP_LEFT_MUGSHOT;
                        else
                            level.layout[a][b] = RoomType::R_SHOP_LEFT;
                        return;
                    }
                } else if (a == 1) {
                    if (level.layout[a - 1][b] != RoomType::R_CLOSED &&
                        level.layout[a + 1][b] != RoomType::R_CLOSED) {

                        if (rand() % 2 == 0)
                            level.layout[a][b] = RoomType::R_SHOP_LEFT;
                        else
                            level.layout[a][b] = RoomType::R_SHOP_RIGHT;

                        return;
                    }
                }
            }
        }
    }
}

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

    if (splash_type == ON_LEVEL_DONE || splash_type == SCORES || splash_type == MAIN_MENU) {
        offset_on_upper_screen = true;

        if (splash_type == MAIN_MENU)
            memcpy(tab, main_menu_upper, sizeof(main_menu_upper));
        else if (splash_type == ON_LEVEL_DONE)
            memcpy(tab, on_level_done_upper, sizeof(on_level_done_upper));
        else if (splash_type == SCORES)
            memcpy(tab, scores_upper, sizeof(scores_upper));
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
void Level::get_first_tile_of_given_type(MapTileType mapTileType, MapTile *&m) const {
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
    {
        for (int b = 0; b < 32; b++)
        {
            delete map_tiles[a][b];
        }
    }

    if (_render_entity.id != Renderer::INVALID_ENTITY)
    {
        Renderer::instance().mark_for_removal(_render_entity.id);
    }
}

void Level::batch_vertices()
{
    _mesh.clear();
    _indices.clear();

    std::size_t tile_counter = 0;
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

            const auto mesh = tile.get_quad_vertices(x, y);
            const auto indices = tile.get_quad_indices(tile_counter);

            std::copy(mesh.begin(), mesh.end(), std::back_inserter(_mesh));
            std::copy(indices.begin(), indices.end(), std::back_inserter(_indices));

            tile_counter++;
        }
    }
}

void Level::add_render_entity()
{
    auto& renderer = Renderer::instance();
    _render_entity.vertices = _mesh.data();
    _render_entity.indices = _indices.data();
    _render_entity.indices_count = _indices.size();
    _render_entity.texture = TextureBank::instance().get_texture(TextureType::CAVE_LEVEL_TILES);
    _render_entity.id = renderer.add_entity(_render_entity);
}

void Level::generate_cave_background()
{
    int random_offset = std::rand();

    for (int x = 0; x < 32; x++)
    {
        for (int y = 0; y < 32; y++)
        {
            if (map_tiles[x][y]->mapTileType == MapTileType::NOTHING)
            {
                if (x % 4)
                {
                    random_offset = std::rand();
                }

                int type_index = x + y + random_offset;
                type_index = type_index % 4;

                const auto type = static_cast<MapTileType>(type_index + static_cast<int>(MapTileType::CAVE_BG_1));
                map_tiles[x][y]->mapTileType = type;
                map_tiles[x][y]->match_tile(type);
            }
        }
    }
}
