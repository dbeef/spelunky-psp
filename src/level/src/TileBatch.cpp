//
// Created by xdbeef on 04.03.18.
//

#include <cmath>
#include <random>
#include <cstdlib>
#include <cstring>
#include <algorithm>

#include "Level.hpp"
#include "Renderer.hpp"
#include "TileBatch.hpp"
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

using namespace Consts;

namespace
{
    int get_random_number()
    {
        static std::random_device random_device;
        std::default_random_engine engine(random_device());
        std::uniform_int_distribution<int> uniform_dist(1, 6);
        return uniform_dist(engine);
    }

    enum Direction
    {
        LEFT,
        RIGHT,
        DOWN
    };
}

/**
 * Used in the process of placing rooms.
 * !\relates generate_new_rooms
 */
void obtain_new_direction(int curr_x, Direction &direction)
{
    if (curr_x == 0)
        //we're on the left side of the map, so go right
        direction = Direction::RIGHT;
    else if (curr_x == 2)
        //we're on the right side of the map, so go left
        direction = Direction::LEFT;
    else
        //we're in the middle, so make a guess where should we gow now
        direction = static_cast<Direction>(get_random_number() % 2); //left or right
}

void TileBatch::generate_new_level_layout()
{
    auto &level = Level::instance().get_tile_batch();

    //clean current layout
    level.clean_map_layout();
    for (auto &room_type : level.layout)
        for (RoomType &b : room_type)
            //not visited rooms are of CLOSED type by default
            b = RoomType::CLOSED;

    //generate new seed for the random number generator
//    srand(timerElapsed(1));

    //set starting position to the random room in the most upper row
    int curr_x = get_random_number() % 3;
    int curr_y = ROOMS_HEIGHT - 1;
    //direction represents where the generator will go in the next loop iteration
    Direction direction;
    obtain_new_direction(curr_x, direction);

    bool exit_placed = false;

    //set the starting room as an entrance room
    level.layout[curr_x][curr_y] = RoomType::ENTRANCE;

    //while we're on the very bottom floor or higher, do
    while (curr_y >= 0)
    {

        if (direction == Direction::LEFT || direction == Direction::RIGHT)
        {

            if ((direction == Direction::LEFT && curr_x == 0) || (direction == Direction::RIGHT && curr_x == 2))
            {
                //our direction is to go left, but we're already on the left side of the map, so go down
                direction = Direction::DOWN;
            } else
            {

                if (direction == Direction::LEFT)
                    //our direction is to go left, and we're not on the left side of the map yet
                    curr_x--;
                else
                    //same, if right side
                    curr_x++;

                if (curr_y == 0 && !exit_placed && get_random_number() % 2 == 0)
                {
                    //we're on the most bottom floor, we didn't plant an exit yet and we've guessed that's the place
                    exit_placed = true;
                    level.layout[curr_x][curr_y] = RoomType::EXIT;
                } else
                    level.layout[curr_x][curr_y] = RoomType::LEFT_RIGHT;

                if (get_random_number() % 3 == 2)
                    //random chance that we change our direction to go down in the next iteration
                    direction = Direction::DOWN;
            }

        } else if (direction == Direction::DOWN)
        {

            if (curr_y > 0)
            {

                level.layout[curr_x][curr_y] = RoomType::LEFT_RIGHT_DOWN;
                curr_y--;
                level.layout[curr_x][curr_y] = RoomType::LEFT_RIGHT_UP;

                if (curr_y == 0 && !exit_placed && get_random_number() % 2 == 0)
                {
                    //if we're on the very bottom floor, no exit planted yet and a guess tells us so, place an exit
                    exit_placed = true;
                    level.layout[curr_x][curr_y] = RoomType::EXIT;
                }

                obtain_new_direction(curr_x, direction);
            } else
            {

                if (!exit_placed)
                    //we're on the very bottom floor, didn't plant an exit yet and we're
                    //done with iterating through map, so plant an exit
                    level.layout[curr_x][curr_y] = RoomType::EXIT;

                break;
            }

        }
    }

    //TODO more post-generation effects, i.e if there's a column of '0' type rooms, then make a snake well
    place_an_altar();
    place_a_shop();
}

void TileBatch::place_an_altar()
{
    auto &level = Level::instance().get_tile_batch();
    for (int x = 0; x < ROOMS_WIDTH; x++)
    {
        for (int y = 0; y < ROOMS_HEIGHT; y++)
        {
            if (level.layout[x][y] == RoomType::CLOSED)
            {
                level.layout[x][y] = RoomType::ALTAR;
                return;
            }
        }
    }
}

void TileBatch::place_a_shop()
{
    auto &level = Level::instance().get_tile_batch();

    for (int x = 0; x < ROOMS_WIDTH; x++)
    {
        for (int y = 0; y < ROOMS_HEIGHT; y++)
        {
            if (level.layout[x][y] == RoomType::CLOSED)
            {
                if (x == 0)
                {
                    if (level.layout[x + 1][y] != RoomType::CLOSED)
                    {
//                        TODO: Uncomment once shopkeeper is implemented.
//                        if (GameState::instance().robbed_or_killed_shopkeeper)
//                        if (false)
//                            level.layout[a][b] = RoomType::SHOP_RIGHT_MUGSHOT;
//                        else
                        level.layout[x][y] = RoomType::SHOP_RIGHT;
                        return;
                    }
                }
                else if (x == 2)
                {
                    if (level.layout[x - 1][y] != RoomType::CLOSED)
                    {
//                        TODO: Uncomment once shopkeeper is implemented.
//                        if (GameState::instance().robbed_or_killed_shopkeeper)
//                        if (false)
//                            level.layout[a][b] = RoomType::SHOP_LEFT_MUGSHOT;
//                        else

                        // Placed a shop, can return.
                        level.layout[x][y] = RoomType::SHOP_LEFT;
                        return;
                    }
                }
                else if (x == 1)
                {
                    if (level.layout[x - 1][y] != RoomType::CLOSED &&
                        level.layout[x + 1][y] != RoomType::CLOSED)
                    {

                        if (get_random_number() % 2 == 0)
                        {
                            level.layout[x][y] = RoomType::SHOP_LEFT;
                        } else
                        {
                            level.layout[x][y] = RoomType::SHOP_RIGHT;
                        }

                        // Placed a shop, can return.
                        return;
                    }
                }
            }
        }
    }
}

void TileBatch::generate_frame()
{
    // Upper row
    for (int x = 0; x < LEVEL_WIDTH_TILES; x++)
    {
        map_tiles[x][0]->match_tile(MapTileType::CAVE_REGULAR);
        map_tiles[x][0]->destroyable = false;
        map_tiles[x][0]->x = x;
        map_tiles[x][0]->y = 0;
        map_tiles[x][0]->exists = true;
    }

    // Left row
    for (int y = 0; y < LEVEL_HEIGHT_TILES; y++)
    {
        map_tiles[0][y]->match_tile(MapTileType::CAVE_REGULAR);
        map_tiles[0][y]->destroyable = false;
        map_tiles[0][y]->x = 0;
        map_tiles[0][y]->y = y;
        map_tiles[0][y]->exists = true;
    }

    // Bottom row
    for (int y = 0; y < LEVEL_HEIGHT_TILES; y++)
    {
        map_tiles[LEVEL_WIDTH_TILES - 1][y]->match_tile(MapTileType::CAVE_REGULAR);
        map_tiles[LEVEL_WIDTH_TILES - 1][y]->destroyable = false;
        map_tiles[LEVEL_WIDTH_TILES - 1][y]->x = LEVEL_WIDTH_TILES - 1;
        map_tiles[LEVEL_WIDTH_TILES - 1][y]->y = y;
        map_tiles[LEVEL_WIDTH_TILES - 1][y]->exists = true;
    }

    // Right row
    for (int x = 0; x < LEVEL_WIDTH_TILES; x++)
    {
        map_tiles[x][LEVEL_HEIGHT_TILES - 1]->match_tile(MapTileType::CAVE_REGULAR);
        map_tiles[x][LEVEL_HEIGHT_TILES - 1]->destroyable = false;
        map_tiles[x][LEVEL_HEIGHT_TILES - 1]->x = x;
        map_tiles[x][LEVEL_HEIGHT_TILES - 1]->y = LEVEL_HEIGHT_TILES - 1;
        map_tiles[x][LEVEL_HEIGHT_TILES - 1]->exists = true;
    }
}

/**
 * Initialises every MapTile in the map_tiles[][] array with its position on the map,
 * so it could be allowed to call LevelGenerator::tiles_to_map.
 * It utilizes given splash screen type, to know if any tile should be planted on the place it iterates,
 * and what MapTileType it should have.
 */
void TileBatch::initialise_tiles_from_splash_screen(SplashScreenType splash_type)
{

    int tab[SPLASH_SCREEN_HEIGHT][SPLASH_SCREEN_WIDTH];
    bool offset_on_upper_screen = false;

    if (splash_type == SplashScreenType::LEVEL_SUMMARY || splash_type == SplashScreenType::SCORES ||
        splash_type == SplashScreenType::MAIN_MENU)
    {
        offset_on_upper_screen = true;

        if (splash_type == SplashScreenType::MAIN_MENU)
            memcpy(tab, main_menu, sizeof(main_menu));
        else if (splash_type == SplashScreenType::LEVEL_SUMMARY)
            memcpy(tab, level_summary, sizeof(level_summary));
        else if (splash_type == SplashScreenType::SCORES)
            memcpy(tab, scores, sizeof(scores));
    }

    //Now we initialise every tile in the splash screen and give it a map_index, which describes its location
    for (int tab_y = 0; tab_y < SPLASH_SCREEN_HEIGHT; tab_y++)
    {
        for (int tab_x = 0; tab_x < SPLASH_SCREEN_WIDTH; tab_x++)
        {

//            if (tab[tab_y][tab_x] != 0) {

            //offset to the position in current room
            int room_offset =
                    static_cast<int>(
                            2 * ROOM_TILE_HEIGHT_SPLASH_SCREEN *
                            LINE_WIDTH * ((ROOMS_HEIGHT - offset_on_upper_screen) - 1) - 4 * OFFSET_Y);
            //pos x and y in pixels of the tile in the current room
            int pos_x = static_cast<int>((tab_x * 2) / 2);
            //NDS engine has different coordinate system than our room layout map,
            //so we invert the Y axis by ((ROOMS_Y - offset_on_upper_screen) - 1)
            int pos_y = static_cast<int>(
                    tab_y + ROOM_TILE_HEIGHT_SPLASH_SCREEN * ((ROOMS_HEIGHT - offset_on_upper_screen) - 1) - 4);

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

void TileBatch::initialise_tiles_from_room_layout()
{
    //a room, 10x10 tiles
    int tab[10][10];
    int r;

    //iterate through every room we have
    for (int room_y = ROOMS_HEIGHT - 1; room_y >= 0; room_y--)
    {
        for (int room_x = 0; room_x < ROOMS_WIDTH; room_x++)
        {

            //basing on the room type, randomly select a variation of this room
            //and copy it to the temporary tab[10][10] array
            r = get_random_number() % 6;
            layout_room_ids[room_x][room_y] = r; //-1 if completely disabling NPC's in this room

            //copying specific room variation
            switch (layout[room_x][room_y])
            {
                case RoomType::CLOSED:
                    memcpy(tab, closed_rooms[r], sizeof(closed_rooms[r]));
                    break;
                case RoomType::LEFT_RIGHT:
                    memcpy(tab, left_right_rooms[r], sizeof(left_right_rooms[r]));
                    break;
                case RoomType::LEFT_RIGHT_DOWN:
                    memcpy(tab, left_right_down_rooms[r], sizeof(left_right_down_rooms[r]));
                    break;
                case RoomType::LEFT_RIGHT_UP:
                    memcpy(tab, left_right_up_rooms[r], sizeof(left_right_up_rooms[r]));
                    break;
                case RoomType::ENTRANCE:
                    memcpy(tab, entrance_room[r], sizeof(entrance_room[r]));
                    break;
                case RoomType::EXIT:
                    memcpy(tab, exit_room[r], sizeof(exit_room[r]));
                    break;
                case RoomType::SHOP_LEFT:
                    memcpy(tab, shops[0], sizeof(shops[0]));
                    break;
                case RoomType::SHOP_RIGHT:
                    memcpy(tab, shops[1], sizeof(shops[1]));
                    break;
                case RoomType::SHOP_LEFT_MUGSHOT:
                    memcpy(tab, shops_mugshots[0], sizeof(shops_mugshots[0]));
                    break;
                case RoomType::SHOP_RIGHT_MUGSHOT:
                    memcpy(tab, shops_mugshots[1], sizeof(shops_mugshots[1]));
                    break;
                case RoomType::ALTAR:
                    memcpy(tab, altar_room[0], sizeof(altar_room[1]));
                    break;
                default:
                    break;
            }

            //Now we initialise every tile in map and give it a map_index, which describes its location
            for (int tab_y = 0; tab_y < ROOM_TILE_HEIGHT_GAME; tab_y++)
            {
                for (int tab_x = 0; tab_x < ROOM_TILE_WIDTH_GAME; tab_x++)
                {


                    //pos x and y in pixels of the tile in the current room
                    int pos_x = static_cast<int>((OFFSET_X + tab_x * 2 + 2 * ROOM_TILE_WIDTH_GAME * room_x) / 2);
                    //NDS engine has different coordinate system than our room layout map,
                    //so we invert the Y axis by ((ROOMS_Y - room_y) - 1))
                    int pos_y = static_cast<int>(
                            (OFFSET_X + tab_y * 2 + 2 * ROOM_TILE_HEIGHT_GAME * ((ROOMS_HEIGHT - room_y) - 1)) / 2);

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

void TileBatch::get_first_tile_of_given_type(MapTileType map_tile_type, MapTile *&out) const
{
    for (int x = 0; x < LEVEL_WIDTH_TILES; x++)
    {
        for (int y = 0; y < LEVEL_HEIGHT_TILES; y++)
        {
            if (map_tiles[x][y]->exists && map_tiles[x][y]->map_tile_type == map_tile_type)
            {
                out = map_tiles[x][y];
                return;
            }
        }
    }
}

void TileBatch::clean_map_layout()
{
    for (int x = 0; x < 32; x++)
    {
        for (int y = 0; y < 32; y++)
        {
            map_tiles[x][y]->exists = false;
            map_tiles[x][y]->destroyable = true;
        }
    }
}

TileBatch::TileBatch()
{
    for (int x = 0; x < 32; x++)
    {
        for (int y = 0; y < 32; y++)
        {
            map_tiles[x][y] = new MapTile();
            map_tiles[x][y]->x = x;
            map_tiles[x][y]->y = y;
        }
    }
}

TileBatch::~TileBatch()
{
    for (int x = 0; x < 32; x++)
    {
        for (int y = 0; y < 32; y++)
        {
            delete map_tiles[x][y];
        }
    }

    if (_render_entity.id != Renderer::INVALID_ENTITY)
    {
        Renderer::instance().mark_for_removal(_render_entity.id, Renderer::EntityType::MODEL_VIEW_SPACE);
    }
}

void TileBatch::batch_vertices()
{
    _mesh.clear();
    _indices.clear();

    std::size_t tile_counter = 0;
    // FIXME: Rewrite level generator for more sane convention of storing tiles.
    // iterating from left-lower corner of the room to the right-upper (spelunky-ds convention)
    for (int x = 0; x < Consts::LEVEL_WIDTH_TILES; x++)
    {
        for (int y = 0; y < Consts::LEVEL_HEIGHT_TILES; y++)
        {

            MapTile *t = map_tiles[x][y];

            // FIXME: Remove x/y fields from MapTile, as they are redundant.
            assert(map_tiles[x][y]->x == x);
            assert(map_tiles[x][y]->y == y);

            auto tile_type = static_cast<int>(t->map_tile_type);
            const auto &tile = TextureBank::instance().get_region(TextureType::CAVE_LEVEL_TILES, tile_type);

            const auto mesh = tile.get_quad_vertices(x, y);
            const auto indices = tile.get_quad_indices(tile_counter);

            std::copy(mesh.begin(), mesh.end(), std::back_inserter(_mesh));
            std::copy(indices.begin(), indices.end(), std::back_inserter(_indices));

            tile_counter++;
        }
    }
}

void TileBatch::add_render_entity()
{
    auto &renderer = Renderer::instance();
    _render_entity.vertices = _mesh.data();
    _render_entity.indices = _indices.data();
    _render_entity.indices_count = _indices.size();
    _render_entity.texture = TextureBank::instance().get_texture(TextureType::CAVE_LEVEL_TILES);
    _render_entity.id = renderer.add_entity(_render_entity, Renderer::EntityType::MODEL_VIEW_SPACE);
}

void TileBatch::generate_cave_background()
{
    int random_offset = get_random_number();

    for (int x = 0; x < 32; x++)
    {
        for (int y = 0; y < 32; y++)
        {
            if (map_tiles[x][y]->map_tile_type == MapTileType::NOTHING)
            {
                if (x % 4)
                {
                    random_offset = get_random_number();
                }

                int type_index = x + y + random_offset;
                type_index = type_index % 4;

                const auto type = static_cast<MapTileType>(type_index + static_cast<int>(MapTileType::CAVE_BG_1));
                map_tiles[x][y]->map_tile_type = type;
                map_tiles[x][y]->match_tile(type);
            }
        }
    }
}
