//
// Created by xdbeef on 04.03.18.
//

#include <cmath>
#include <random>
#include <cstdlib>
#include <cstring>
#include <algorithm>

#include "NeighbouringTiles.hpp"
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
    enum class Direction : std::uint16_t
    {
        LEFT = 0,
        RIGHT,
        DOWN
    };

    int get_random_number()
    {
        static std::random_device random_device;
        std::default_random_engine engine(random_device());
        std::uniform_int_distribution<int> uniform_dist(1, 6);
        return uniform_dist(engine);
    }

    void obtain_new_direction(int curr_x, Direction &direction)
    {
        if (curr_x == 0)
        {
            // We're on the left side of the map, so go right:
            direction = Direction::RIGHT;
        }
        else if (curr_x == ROOMS_COUNT_WIDTH - 1)
        {
            // We're on the right side of the map, so go left:
            direction = Direction::LEFT;
        }
        else
        {
            // We're in the middle, so make a guess where should we go now (only left/right allowed):
            direction = static_cast<Direction>(get_random_number() % 2);
        }
    }
}

void TileBatch::generate_new_level_layout()
{
    clean_map_layout();

    // Set starting position to the random room in the upper-most row:
    int curr_x = get_random_number() % ROOMS_COUNT_WIDTH;
    int curr_y = ROOMS_COUNT_HEIGHT - 1;

    // Direction represents where the generator will go in the next loop iteration.
    Direction direction;
    obtain_new_direction(curr_x, direction);

    bool exit_placed = false;

    // Set the starting room as an entrance room
    layout[curr_x][curr_y] = RoomType::ENTRANCE;

    // While we're on the very bottom floor or higher, do:
    while (curr_y >= 0)
    {
        if (direction == Direction::LEFT || direction == Direction::RIGHT)
        {
            if ((direction == Direction::LEFT && curr_x == 0) || (direction == Direction::RIGHT && curr_x == ROOMS_COUNT_WIDTH - 1))
            {
                // Our direction is to go left, but we're already on the left side of the map, so go down:
                direction = Direction::DOWN;
            }
            else
            {
                if (direction == Direction::LEFT)
                {
                    // Our direction is to go left, and we're not on the left side of the map yet:
                    curr_x--;
                }
                else
                {
                    // Same, if right side:
                    curr_x++;
                }

                if (curr_y == 0 && !exit_placed && get_random_number() % 2 == 0)
                {
                    // We're on the most bottom floor, we didn't plant an exit yet and we've guessed that's the place:
                    exit_placed = true;
                    layout[curr_x][curr_y] = RoomType::EXIT;
                }
                else
                {
                    layout[curr_x][curr_y] = RoomType::LEFT_RIGHT;
                }

                if (get_random_number() % 3 == 2)
                {
                    // Random chance that we change our direction to go down in the next iteration:
                    direction = Direction::DOWN;
                }
            }
        }
        else if (direction == Direction::DOWN)
        {
            if (curr_y > 0)
            {
                layout[curr_x][curr_y] = RoomType::LEFT_RIGHT_DOWN;
                curr_y--;
                layout[curr_x][curr_y] = RoomType::LEFT_RIGHT_UP;

                if (curr_y == 0 && !exit_placed && get_random_number() % 2 == 0)
                {
                    // If we're on the very bottom floor, no exit planted yet and a guess tells us so, place an exit:
                    exit_placed = true;
                    layout[curr_x][curr_y] = RoomType::EXIT;
                }

                obtain_new_direction(curr_x, direction);
            }
            else
            {
                if (!exit_placed)
                {
                    // We're on the very bottom floor, didn't plant an exit yet and we're
                    // done with iterating through map, so plant an exit:
                    layout[curr_x][curr_y] = RoomType::EXIT;
                }
                break;
            }
        }
    }

    // Post-generation effects:
    place_an_altar();
    place_a_shop();
}

void TileBatch::place_an_altar()
{
    auto &level = Level::instance().get_tile_batch();
    for (int x = 0; x < ROOMS_COUNT_WIDTH; x++)
    {
        for (int y = 0; y < ROOMS_COUNT_HEIGHT; y++)
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

    for (int x = 0; x < ROOMS_COUNT_WIDTH; x++)
    {
        for (int y = 0; y < ROOMS_COUNT_HEIGHT; y++)
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
    }

    // Left row
    for (int y = 0; y < LEVEL_HEIGHT_TILES; y++)
    {
        map_tiles[0][y]->match_tile(MapTileType::CAVE_REGULAR);
        map_tiles[0][y]->destroyable = false;
        map_tiles[0][y]->x = 0;
        map_tiles[0][y]->y = y;
    }

    // Bottom row
    for (int y = 0; y < LEVEL_HEIGHT_TILES; y++)
    {
        map_tiles[LEVEL_WIDTH_TILES - 1][y]->match_tile(MapTileType::CAVE_REGULAR);
        map_tiles[LEVEL_WIDTH_TILES - 1][y]->destroyable = false;
        map_tiles[LEVEL_WIDTH_TILES - 1][y]->x = LEVEL_WIDTH_TILES - 1;
        map_tiles[LEVEL_WIDTH_TILES - 1][y]->y = y;
    }

    // Right row
    for (int x = 0; x < LEVEL_WIDTH_TILES; x++)
    {
        map_tiles[x][LEVEL_HEIGHT_TILES - 1]->match_tile(MapTileType::CAVE_REGULAR);
        map_tiles[x][LEVEL_HEIGHT_TILES - 1]->destroyable = false;
        map_tiles[x][LEVEL_HEIGHT_TILES - 1]->x = x;
        map_tiles[x][LEVEL_HEIGHT_TILES - 1]->y = LEVEL_HEIGHT_TILES - 1;
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

    int tab[SPLASH_SCREEN_HEIGHT_TILES][SPLASH_SCREEN_WIDTH_TILES];
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
    for (int tab_y = 0; tab_y < SPLASH_SCREEN_HEIGHT_TILES; tab_y++)
    {
        for (int tab_x = 0; tab_x < SPLASH_SCREEN_WIDTH_TILES; tab_x++)
        {

//            if (tab[tab_y][tab_x] != 0) {

            //offset to the position in current room
            //pos x and y in pixels of the tile in the current room
            int pos_x = static_cast<int>((tab_x * 2) / 2);
            //NDS engine has different coordinate system than our room layout map,
            //so we invert the Y axis by ((ROOMS_Y - offset_on_upper_screen) - 1)
            int pos_y = static_cast<int>(
                    tab_y + SPLASH_SCREEN_HEIGHT_TILES * ((ROOMS_COUNT_HEIGHT - offset_on_upper_screen) - 1) - 4);

            map_tiles[pos_x][pos_y]->match_tile(static_cast<MapTileType>(tab[tab_y][tab_x]));
            map_tiles[pos_x][pos_y]->x = pos_x;
            map_tiles[pos_x][pos_y]->y = pos_y;
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
    int tab[ROOM_WIDTH_TILES][ROOM_HEIGHT_TILES];
    int r;

    //iterate through every room we have
    for (int room_y = ROOMS_COUNT_HEIGHT - 1; room_y >= 0; room_y--)
    {
        for (int room_x = 0; room_x < ROOMS_COUNT_WIDTH; room_x++)
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
            for (int tab_y = 0; tab_y < ROOM_HEIGHT_TILES; tab_y++)
            {
                for (int tab_x = 0; tab_x < ROOM_WIDTH_TILES; tab_x++)
                {
                    const int OFFSET_X = 2; //Offset of 2 tiles, 8 px each

                    //pos x and y in pixels of the tile in the current room
                    int pos_x = static_cast<int>((OFFSET_X + tab_x * 2 + 2 * ROOM_WIDTH_TILES * room_x) / 2);
                    //NDS engine has different coordinate system than our room layout map,
                    //so we invert the Y axis by ((ROOMS_Y - room_y) - 1))
                    int pos_y = static_cast<int>(
                            (OFFSET_X + tab_y * 2 + 2 * ROOM_HEIGHT_TILES * ((ROOMS_COUNT_HEIGHT - room_y) - 1)) / 2);

                    map_tiles[pos_x][pos_y]->match_tile(static_cast<MapTileType>(tab[tab_y][tab_x]));

                    map_tiles[pos_x][pos_y]->x = pos_x;
                    map_tiles[pos_x][pos_y]->y = pos_y;
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
            if (map_tiles[x][y]->map_tile_type == map_tile_type)
            {
                out = map_tiles[x][y];
                return;
            }
        }
    }
}

void TileBatch::clean_map_layout()
{
    for (int x = 0; x < LEVEL_WIDTH_TILES; x++)
    {
        for (int y = 0; y < LEVEL_HEIGHT_TILES; y++)
        {
            map_tiles[x][y]->destroyable = true;
        }
    }

    for (int x = 0; x < ROOMS_COUNT_WIDTH; x++)
    {
        for (int y = 0; y < ROOMS_COUNT_HEIGHT; y++)
        {
            layout[x][y] = RoomType::CLOSED;
        }
    }
}

TileBatch::TileBatch()
{
    for (int x = 0; x < LEVEL_WIDTH_TILES; x++)
    {
        for (int y = 0; y < LEVEL_HEIGHT_TILES; y++)
        {
            map_tiles[x][y] = new MapTile();
            map_tiles[x][y]->x = x;
            map_tiles[x][y]->y = y;
        }
    }
}

TileBatch::~TileBatch()
{
    for (int x = 0; x < LEVEL_WIDTH_TILES; x++)
    {
        for (int y = 0; y < LEVEL_HEIGHT_TILES; y++)
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
    for (int x = 0; x < LEVEL_WIDTH_TILES; x++)
    {
        for (int y = 0; y < LEVEL_HEIGHT_TILES; y++)
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

    for (int x = 0; x < LEVEL_WIDTH_TILES; x++)
    {
        for (int y = 0; y < LEVEL_HEIGHT_TILES; y++)
        {
            if (map_tiles[x][y]->map_tile_type == MapTileType::NOTHING)
            {
                if (x % 4)
                {
                    random_offset = get_random_number();
                }

                int type_index = x + y + random_offset;
                type_index = type_index % 4;

                const auto type = static_cast<MapTileType >(type_index + static_cast<int>(MapTileType::CAVE_BG_1));
                map_tiles[x][y]->map_tile_type = type;
                map_tiles[x][y]->match_tile(type);
            }
        }
    }
}

void TileBatch::get_neighbouring_tiles(float x, float y, MapTile *out_neighboring_tiles[9]) const
{
    std::uint16_t x_tiles = std::floor(x + (MapTile::PHYSICAL_WIDTH / 2.0f));
    std::uint16_t y_tiles = std::floor(y + (MapTile::PHYSICAL_HEIGHT / 2.0f));

    assert(x_tiles < Consts::LEVEL_WIDTH_TILES);
    assert(y_tiles < Consts::LEVEL_HEIGHT_TILES);

    MapTile *left_middle,
            *right_middle,
            *up_middle,
            *down_middle,
            *center,
            *left_up,
            *right_up,
            *left_down,
            *right_down;

    left_middle = x_tiles - 1 >= 0 ? map_tiles[x_tiles - 1][y_tiles] : nullptr;
    right_middle = x_tiles + 1 < Consts::LEVEL_WIDTH_TILES ? map_tiles[x_tiles + 1][y_tiles] : nullptr;
    up_middle = y_tiles - 1 >= 0 ? map_tiles[x_tiles][y_tiles - 1] : nullptr;
    down_middle = y_tiles + 1 < Consts::LEVEL_HEIGHT_TILES ? map_tiles[x_tiles][y_tiles + 1] : nullptr;
    center = map_tiles[x_tiles][y_tiles];
    left_up = x_tiles - 1 >= 0 && y_tiles - 1 >= 0 ? map_tiles[x_tiles - 1][y_tiles - 1] : nullptr;
    right_up = x_tiles + 1 < Consts::LEVEL_WIDTH_TILES && y_tiles - 1 >= 0 ? map_tiles[x_tiles + 1][y_tiles - 1] : nullptr;
    left_down = x_tiles - 1 >= 0 && y_tiles + 1 < Consts::LEVEL_HEIGHT_TILES ? map_tiles[x_tiles - 1][y_tiles + 1] : nullptr;
    right_down = x_tiles + 1 < Consts::LEVEL_WIDTH_TILES && y_tiles + 1 < Consts::LEVEL_HEIGHT_TILES ? map_tiles[x_tiles + 1][y_tiles + 1] : nullptr;

    out_neighboring_tiles[static_cast<std::uint16_t>(NeighbouringTiles::LEFT_MIDDLE)] = left_middle;
    out_neighboring_tiles[static_cast<std::uint16_t>(NeighbouringTiles::RIGHT_MIDDLE)] = right_middle;
    out_neighboring_tiles[static_cast<std::uint16_t>(NeighbouringTiles::UP_MIDDLE)] = up_middle;
    out_neighboring_tiles[static_cast<std::uint16_t>(NeighbouringTiles::DOWN_MIDDLE)] = down_middle;
    out_neighboring_tiles[static_cast<std::uint16_t>(NeighbouringTiles::CENTER)] = center;
    out_neighboring_tiles[static_cast<std::uint16_t>(NeighbouringTiles::LEFT_UP)] = left_up;
    out_neighboring_tiles[static_cast<std::uint16_t>(NeighbouringTiles::RIGHT_UP)] = right_up;
    out_neighboring_tiles[static_cast<std::uint16_t>(NeighbouringTiles::LEFT_DOWN)] = left_down;
    out_neighboring_tiles[static_cast<std::uint16_t>(NeighbouringTiles::RIGHT_DOWN)] = right_down;
}
