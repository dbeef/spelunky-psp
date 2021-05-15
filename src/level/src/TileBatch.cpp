//
// Created by xdbeef on 04.03.18.
//

#include <cmath>
#include <random>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <TextureType.hpp>
#include <RenderingLayer.hpp>

#include "TextureBank.hpp"
#include "NeighbouringTiles.hpp"
#include "Level.hpp"
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
// FIXME
#include "../../game-loop/include/components/generic/MeshComponent.hpp"

using namespace Consts;

namespace
{
    enum class Direction : std::uint16_t
    {
        LEFT = 0,
        RIGHT,
        DOWN
    };

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
            direction = static_cast<Direction>(std::rand() % 2);
        }
    }
}

void TileBatch::generate_new_level_layout(const LevelGeneratorParams& params)
{
    // Clean the previous layout:
    for (int x = 0; x < ROOMS_COUNT_WIDTH; x++)
    {
        for (int y = 0; y < ROOMS_COUNT_HEIGHT; y++)
        {
            _layout[x][y] = RoomType::CLOSED;
        }
    }

    // Set starting position to the random room in the upper-most row:
    int curr_x = std::rand() % ROOMS_COUNT_WIDTH;
    int curr_y = ROOMS_COUNT_HEIGHT - 1;

    // Direction represents where the generator will go in the next loop iteration.
    Direction direction;
    obtain_new_direction(curr_x, direction);

    bool exit_placed = false;

    // Set the starting room as an entrance room
    _layout[curr_x][curr_y] = RoomType::ENTRANCE;

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

                if (curr_y == 0 && !exit_placed && std::rand() % 2 == 0)
                {
                    // We're on the most bottom floor, we didn't plant an exit yet and we've guessed that's the place:
                    exit_placed = true;
                    _layout[curr_x][curr_y] = RoomType::EXIT;
                }
                else
                {
                    _layout[curr_x][curr_y] = RoomType::LEFT_RIGHT;
                }

                if (std::rand() % 3 == 2)
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
                _layout[curr_x][curr_y] = RoomType::LEFT_RIGHT_DOWN;
                curr_y--;
                _layout[curr_x][curr_y] = RoomType::LEFT_RIGHT_UP;

                if (curr_y == 0 && !exit_placed && std::rand() % 2 == 0)
                {
                    // If we're on the very bottom floor, no exit planted yet and a guess tells us so, place an exit:
                    exit_placed = true;
                    _layout[curr_x][curr_y] = RoomType::EXIT;
                }

                obtain_new_direction(curr_x, direction);
            }
            else
            {
                if (!exit_placed)
                {
                    // We're on the very bottom floor, didn't plant an exit yet and we're
                    // done with iterating through map, so plant an exit:
                    _layout[curr_x][curr_y] = RoomType::EXIT;
                }
                break;
            }
        }
    }

    // Post-generation effects:
    place_an_altar();
    place_a_shop(params.shopkeeper_robbed);
}

void TileBatch::place_an_altar()
{
    auto &level = Level::instance().get_tile_batch();
    for (int x = 0; x < ROOMS_COUNT_WIDTH; x++)
    {
        for (int y = 0; y < ROOMS_COUNT_HEIGHT; y++)
        {
            if (level._layout[x][y] == RoomType::CLOSED)
            {
                level._layout[x][y] = RoomType::ALTAR;
                return;
            }
        }
    }
}

void TileBatch::place_a_shop(bool shopkeeper_robbed)
{
    auto &level = Level::instance().get_tile_batch();

    for (int x = 0; x < ROOMS_COUNT_WIDTH; x++)
    {
        for (int y = 0; y < ROOMS_COUNT_HEIGHT; y++)
        {
            if (level._layout[x][y] == RoomType::CLOSED)
            {
                if (x == 0)
                {
                    if (level._layout[x + 1][y] != RoomType::CLOSED)
                    {
                        if (shopkeeper_robbed)
                        {
                            level._layout[x][y] = RoomType::SHOP_RIGHT_MUGSHOT;
                        }
                        else
                        {
                            level._layout[x][y] = RoomType::SHOP_RIGHT;
                        }
                        return;
                    }
                }
                else if (x == 2)
                {
                    if (level._layout[x - 1][y] != RoomType::CLOSED)
                    {
                        if (shopkeeper_robbed)
                        {
                            level._layout[x][y] = RoomType::SHOP_LEFT_MUGSHOT;
                        }
                        else
                        {
                            level._layout[x][y] = RoomType::SHOP_LEFT;
                        }
                        return;
                    }
                }
                else if (x == 1)
                {
                    if (level._layout[x - 1][y] != RoomType::CLOSED &&
                        level._layout[x + 1][y] != RoomType::CLOSED)
                    {

                        if (std::rand() % 2 == 0)
                        {
                            level._layout[x][y] = RoomType::SHOP_LEFT;
                        } else
                        {
                            level._layout[x][y] = RoomType::SHOP_RIGHT;
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

void TileBatch::initialise_tiles_from_splash_screen(SplashScreenType splash_type)
{
    int temp[SPLASH_SCREEN_HEIGHT_TILES][SPLASH_SCREEN_WIDTH_TILES];

    switch(splash_type)
    {
        case SplashScreenType::LEVEL_SUMMARY: memcpy(temp, level_summary, sizeof(level_summary)); break;
        case SplashScreenType::SCORES: memcpy(temp, scores, sizeof(scores)); break;
        case SplashScreenType::MAIN_MENU: memcpy(temp, main_menu, sizeof(main_menu)); break;
        default: assert(false); break;
    }

    // Now copy the specific splash screen 2D layout into the tile batch:
    for (int tab_y = 0; tab_y < SPLASH_SCREEN_HEIGHT_TILES; tab_y++)
    {
        for (int tab_x = 0; tab_x < SPLASH_SCREEN_WIDTH_TILES; tab_x++)
        {
            map_tiles[tab_x][tab_y]->match_tile(static_cast<MapTileType>(temp[tab_y][tab_x]));
            map_tiles[tab_x][tab_y]->x = tab_x;
            map_tiles[tab_x][tab_y]->y = tab_y;
        }
    }
}

void TileBatch::initialise_tiles_from_room_layout()
{
    int temp[ROOM_WIDTH_TILES][ROOM_HEIGHT_TILES];

    // Iterate through every room there is:
    for (int room_y = ROOMS_COUNT_HEIGHT - 1; room_y >= 0; room_y--)
    {
        for (int room_x = 0; room_x < ROOMS_COUNT_WIDTH; room_x++)
        {
            switch (_layout[room_x][room_y])
            {
                // Basing on the room type, randomly select a variation of this room
                // and copy it to the temporary array:
                case RoomType::CLOSED:
                {
                    const int possible_variations = std::extent<decltype(closed_rooms)>::value;
                    const int room_index = std::rand() % possible_variations;
                    _layout_room_ids[room_x][room_y] = room_index;
                    memcpy(temp, closed_rooms[room_index], sizeof(closed_rooms[room_index]));
                    break;
                }
                case RoomType::LEFT_RIGHT:
                {
                    const int possible_variations = std::extent<decltype(left_right_rooms)>::value;
                    const int room_index = std::rand() % possible_variations;
                    _layout_room_ids[room_x][room_y] = room_index;
                    memcpy(temp, left_right_rooms[room_index], sizeof(left_right_rooms[room_index]));
                    break;
                }
                case RoomType::LEFT_RIGHT_DOWN:
                {
                    const int possible_variations = std::extent<decltype(left_right_down_rooms)>::value;
                    const int room_index = std::rand() % possible_variations;
                    _layout_room_ids[room_x][room_y] = room_index;
                    memcpy(temp, left_right_down_rooms[room_index], sizeof(left_right_down_rooms[room_index]));
                    break;
                }
                case RoomType::LEFT_RIGHT_UP:
                {
                    const int possible_variations = std::extent<decltype(left_right_up_rooms)>::value;
                    const int room_index = std::rand() % possible_variations;
                    _layout_room_ids[room_x][room_y] = room_index;
                    memcpy(temp, left_right_up_rooms[room_index], sizeof(left_right_up_rooms[room_index]));
                    break;
                }
                case RoomType::ENTRANCE:
                {
                    const int possible_variations = std::extent<decltype(entrance_room)>::value;
                    const int room_index = std::rand() % possible_variations;
                    _layout_room_ids[room_x][room_y] = room_index;
                    memcpy(temp, entrance_room[room_index], sizeof(entrance_room[room_index]));
                    break;
                }
                case RoomType::EXIT:
                {
                    const int possible_variations = std::extent<decltype(exit_room)>::value;
                    const int room_index = std::rand() % possible_variations;
                    _layout_room_ids[room_x][room_y] = room_index;
                    memcpy(temp, exit_room[room_index], sizeof(exit_room[room_index]));
                    break;
                }
                // There are no room variations for the rooms below:
                case RoomType::SHOP_LEFT: memcpy(temp, shops[0], sizeof(shops[0])); break;
                case RoomType::SHOP_RIGHT: memcpy(temp, shops[1], sizeof(shops[1])); break;
                case RoomType::SHOP_LEFT_MUGSHOT: memcpy(temp, shops_mugshots[0], sizeof(shops_mugshots[0])); break;
                case RoomType::SHOP_RIGHT_MUGSHOT: memcpy(temp, shops_mugshots[1], sizeof(shops_mugshots[1])); break;
                case RoomType::ALTAR: memcpy(temp, altar_room[0], sizeof(altar_room[1])); break;
                default: assert(false); break;
            }

            for (int tab_y = 0; tab_y < ROOM_HEIGHT_TILES; tab_y++)
            {
                for (int tab_x = 0; tab_x < ROOM_WIDTH_TILES; tab_x++)
                {
                    // Offset accounting for the undestroyable tiles around the map and XY distance for the specific room:
                    int pos_x = static_cast<int>((2 + tab_x * 2 + 2 * ROOM_WIDTH_TILES * room_x) / 2);
                    int pos_y = static_cast<int>((2 + tab_y * 2 + 2 * ROOM_HEIGHT_TILES * ((ROOMS_COUNT_HEIGHT - room_y) - 1)) / 2);

                    map_tiles[pos_x][pos_y]->match_tile(static_cast<MapTileType>(temp[tab_y][tab_x]));
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
}

void TileBatch::batch_vertices()
{
    _mesh.clear();
    _indices.clear();

    std::size_t tile_counter = 0;

    for (int x = 0; x < LEVEL_WIDTH_TILES; x++)
    {
        for (int y = 0; y < LEVEL_HEIGHT_TILES; y++)
        {
            MapTile *t = map_tiles[x][y];

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

entt::entity TileBatch::add_render_entity(entt::registry &registry)
{
    const auto entity = registry.create();

    MeshComponent mesh_component;

    mesh_component.vertices = _mesh.data();
    mesh_component.indices = _indices.data();
    mesh_component.indices_count = _indices.size();
    mesh_component.texture_id = TextureBank::instance().get_texture(TextureType::CAVE_LEVEL_TILES);
    mesh_component.rendering_layer = RenderingLayer::LAYER_6_TILES;
    mesh_component.camera_type = CameraType::MODEL_VIEW_SPACE;

    registry.emplace<MeshComponent>(entity, mesh_component);

    return entity;
}

void TileBatch::generate_cave_background()
{
    int random_offset = std::rand();

    for (int x = 0; x < LEVEL_WIDTH_TILES; x++)
    {
        for (int y = 0; y < LEVEL_HEIGHT_TILES; y++)
        {
            if (map_tiles[x][y]->map_tile_type == MapTileType::NOTHING)
            {
                if (x % 4)
                {
                    random_offset = std::rand();
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
    std::uint16_t x_tiles = std::floor(x);
    std::uint16_t y_tiles = std::floor(y);

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

LootType TileBatch::get_loot_type_spawned_at(int x_tiles, int y_tiles) const
{
    if (x_tiles == 0 || x_tiles == (Consts::LEVEL_WIDTH_TILES - 1) || y_tiles == 0 || y_tiles == (Consts::LEVEL_HEIGHT_TILES - 1))
    {
        return LootType::NOTHING;
    }

    // Decrease to not include margins around map to the calculation:
    x_tiles--;
    y_tiles--;

    // Inverse Y axis, as layout is stored with different notation: FIXME
    y_tiles = (Consts::LEVEL_HEIGHT_TILES - 2) - y_tiles;

    if (x_tiles < 0 || y_tiles < 0)
    {
        return LootType::NOTHING;
    }

    // Get room ID to which the XY points:
    int room_x = std::floor(x_tiles / ROOM_WIDTH_TILES);
    int room_y = std::floor((y_tiles) / ROOM_HEIGHT_TILES);

    if (room_x >= Consts::ROOMS_COUNT_WIDTH || room_y >= Consts::ROOMS_COUNT_HEIGHT || room_x < 0 || room_y < 0)
    {
        return LootType::NOTHING;
    }

    // Get relative XY in scope of this particular room:
    int x_tiles_room = x_tiles % ROOM_WIDTH_TILES;
    // Inverse Y axis, as room layout is stored with different notation: FIXME
    int y_tiles_room = static_cast<int>(ROOM_HEIGHT_TILES - (y_tiles % ROOM_HEIGHT_TILES));

    if (x_tiles_room >= Consts::ROOM_WIDTH_TILES || y_tiles_room >= Consts::ROOM_HEIGHT_TILES || x_tiles_room < 0 || y_tiles_room < 0)
    {
        return LootType::NOTHING;
    }

    // Get room type:
    const RoomType& room_type = _layout[room_x][room_y];
    int room_id = _layout_room_ids[room_x][room_y];

    switch(room_type)
    {
        case RoomType::CLOSED: return static_cast<LootType>(closed_rooms_loot[room_id][y_tiles_room][x_tiles_room]);
        case RoomType::LEFT_RIGHT: return static_cast<LootType>(left_right_rooms_loot[room_id][y_tiles_room][x_tiles_room]);
        case RoomType::LEFT_RIGHT_DOWN: return static_cast<LootType>(left_right_down_rooms_loot[room_id][y_tiles_room][x_tiles_room]);
        case RoomType::LEFT_RIGHT_UP: return static_cast<LootType>(left_right_up_rooms_loot[room_id][y_tiles_room][x_tiles_room]);
        case RoomType::EXIT: return static_cast<LootType>(exit_rooms_loot[room_id][y_tiles_room][x_tiles_room]);
        case RoomType::ENTRANCE: return static_cast<LootType>(entrance_room_loot[room_id][y_tiles_room][x_tiles_room]);
        case RoomType::SHOP_LEFT: return static_cast<LootType>(shops_loot[0][y_tiles_room][x_tiles_room]);
        case RoomType::SHOP_RIGHT: return static_cast<LootType>(shops_loot[1][y_tiles_room][x_tiles_room]);
        case RoomType::ALTAR: return static_cast<LootType>(altar_loot[y_tiles_room][x_tiles_room]);
        case RoomType::SHOP_LEFT_MUGSHOT:break;
        case RoomType::SHOP_RIGHT_MUGSHOT:break;
        default: assert(false);
    }

    return LootType::NOTHING;
}

// FIXME: Repeating tile coordinate calculation from function above.
NPCType TileBatch::get_npc_type_spawned_at(int x_tiles, int y_tiles) const
{
    if (x_tiles == 0 || x_tiles == (Consts::LEVEL_WIDTH_TILES - 1) || y_tiles == 0 || y_tiles == (Consts::LEVEL_HEIGHT_TILES - 1))
    {
        return NPCType::NOTHING;
    }

    // Decrease to not include margins around map to the calculation:
    x_tiles--;
    y_tiles--;

    // Inverse Y axis, as layout is stored with different notation: FIXME
    y_tiles = (Consts::LEVEL_HEIGHT_TILES - 2) - y_tiles;

    if (x_tiles < 0 || y_tiles < 0)
    {
        return NPCType::NOTHING;
    }

    // Get room ID to which the XY points:
    int room_x = std::floor(x_tiles / ROOM_WIDTH_TILES);
    int room_y = std::floor(y_tiles / ROOM_HEIGHT_TILES);

    if (room_x >= Consts::ROOMS_COUNT_WIDTH || room_y >= Consts::ROOMS_COUNT_HEIGHT || room_x < 0 || room_y < 0)
    {
        return NPCType::NOTHING;
    }

    // Get relative XY in scope of this particular room:
    int x_tiles_room = x_tiles % ROOM_WIDTH_TILES;
    // Inverse Y axis, as room layout is stored with different notation: FIXME
    int y_tiles_room = static_cast<int>(ROOM_HEIGHT_TILES - (y_tiles % ROOM_HEIGHT_TILES));

    if (x_tiles_room >= Consts::ROOM_WIDTH_TILES || y_tiles_room >= Consts::ROOM_HEIGHT_TILES || x_tiles_room < 0 || y_tiles_room < 0)
    {
        return NPCType::NOTHING;
    }

    // Get room type:
    const RoomType& room_type = _layout[room_x][room_y];
    int room_id = _layout_room_ids[room_x][room_y];

    switch(room_type)
    {
        case RoomType::CLOSED: return static_cast<NPCType>(closed_rooms_npc[room_id][y_tiles_room][x_tiles_room]);
        case RoomType::LEFT_RIGHT: return static_cast<NPCType>(left_right_rooms_npc[room_id][y_tiles_room][x_tiles_room]);
        case RoomType::LEFT_RIGHT_DOWN: return static_cast<NPCType>(left_right_down_rooms_npc[room_id][y_tiles_room][x_tiles_room]);
        case RoomType::LEFT_RIGHT_UP: return static_cast<NPCType>(left_right_up_rooms_npc[room_id][y_tiles_room][x_tiles_room]);
        case RoomType::EXIT: return static_cast<NPCType>(exit_rooms_npc[room_id][y_tiles_room][x_tiles_room]);
        case RoomType::ENTRANCE: return static_cast<NPCType>(entrance_room_npc[room_id][y_tiles_room][x_tiles_room]);
        case RoomType::SHOP_LEFT: return static_cast<NPCType>(shops_npc[0][y_tiles_room][x_tiles_room]);
        case RoomType::SHOP_RIGHT: return static_cast<NPCType>(shops_npc[1][y_tiles_room][x_tiles_room]);
        case RoomType::SHOP_LEFT_MUGSHOT:break;
        case RoomType::SHOP_RIGHT_MUGSHOT:break;
        case RoomType::ALTAR:break;
        default: assert(false);
    }

    return NPCType::NOTHING;
}

RoomType TileBatch::get_room_type_at(int x_room, int y_room) const
{
    assert(x_room >= 0 && x_room < ROOMS_COUNT_WIDTH);
    assert(y_room >= 0 && y_room < ROOMS_COUNT_HEIGHT);

    // Reverse notation:
    y_room = std::abs(y_room - (ROOMS_COUNT_HEIGHT - 1));
    return _layout[x_room][y_room];
}
