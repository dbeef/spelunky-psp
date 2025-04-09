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
#include "RoomType.hpp"
#include "components/generic/MeshComponent.hpp"
#include <fstream>
#include <filesystem>
#include "cJSON.h"
#include "assets/Assets.hpp"
#include "assets/Paths.hpp"

using namespace Consts;

struct Room {
    std::string name;
    int width_tiles{};
    int height_tiles{};
    std::vector<MapTileType> tiles;

    Room() = default;

    explicit Room(const std::string &name, int width, int height) : width_tiles(width), height_tiles(height),
                                                                    name(name) {
        tiles.resize(width * height, MapTileType::NOTHING);
    }

    MapTileType &get_xy(int x, int y) {
        return tiles[(y * width_tiles) + x];
    }

    void set_xy(int x, int y, MapTileType value) {
        get_xy(x, y) = value;
    }

    void from_json(cJSON *in_json) {
        auto *name_cjson = cJSON_GetObjectItem(in_json, "name");
        auto *width_tiles_cjson = cJSON_GetObjectItem(in_json, "width_tiles");
        auto *height_tiles_cjson = cJSON_GetObjectItem(in_json, "height_tiles");
        auto *tiles_cjson = cJSON_GetObjectItem(in_json, "tiles");

        assert(cJSON_IsString(name_cjson));
        assert(cJSON_IsNumber(width_tiles_cjson));
        assert(cJSON_IsNumber(height_tiles_cjson));

        name = name_cjson->valuestring;
        width_tiles = width_tiles_cjson->valueint;
        height_tiles = height_tiles_cjson->valueint;

        if (tiles_cjson) {
            auto array_size = cJSON_GetArraySize(tiles_cjson);
            tiles.resize(array_size);
            for (int i = 0; i < array_size; i++) {
                auto *item = cJSON_GetArrayItem(tiles_cjson, i);
                tiles[i] = static_cast<MapTileType>(item->valueint);
            }
        }
    }

    cJSON *to_json() {
        auto *room = cJSON_CreateObject();
        cJSON_AddStringToObject(room, "name", name.c_str());
        cJSON_AddNumberToObject(room, "width_tiles", width_tiles);
        cJSON_AddNumberToObject(room, "height_tiles", height_tiles);

        //            tiles = {MapTileType::STONE_BLOCK, MapTileType::ALTAR_RIGHT};

        auto *rooms_array = cJSON_CreateIntArray(reinterpret_cast<const int *>(tiles.data()), tiles.size());
        cJSON_AddItemToObject(room, "tiles", rooms_array);

        return room;
    }
};

struct RoomGroup {
    std::string name;
    std::vector<Room> rooms;

    const Room &get_room(const char *name) const {
        const auto search_result = std::find_if(rooms.begin(), rooms.end(), [name](const Room &room) {
            return room.name == name;
        });
        if (search_result != rooms.end()) {
            return *search_result;
        } else {
            assert(false);
            return {};
        }
    }


    // TODO: Move out
    void from_json(cJSON *in_json) {
        auto *name_cjson = cJSON_GetObjectItem(in_json, "name");
        assert(cJSON_IsString(name_cjson));
        name = name_cjson->valuestring;

        auto *rooms_cjson = cJSON_GetObjectItem(in_json, "rooms");
        if (rooms_cjson) {
            auto array_size = cJSON_GetArraySize(rooms_cjson);
            rooms.resize(array_size);
            for (int i = 0; i < array_size; i++) {
                auto *item = cJSON_GetArrayItem(rooms_cjson, i);
                rooms[i].from_json(item);
            }
        }
    }

    cJSON *to_json() {
        auto *group = cJSON_CreateObject();
        cJSON_AddStringToObject(group, "name", name.c_str());

        auto *rooms_array = cJSON_CreateArray();

        for (auto &r: rooms) {
            auto *cjson_room = r.to_json();
            cJSON_AddItemToArray(rooms_array, cjson_room);
        }

        cJSON_AddItemToObject(group, "rooms", rooms_array);

        return group;
    }
};

struct LevelEditorSave {
    std::vector<RoomGroup> groups;

    const RoomGroup &get_group(const char *name) const {
        const auto search_result = std::find_if(groups.begin(), groups.end(), [name](const RoomGroup &room_group) {
            return room_group.name == name;
        });
        if (search_result != groups.end()) {
            return *search_result;
        } else {
            assert(false);
            return {};
        }
    }

    void from_json(const char *data) {
        auto *in_json = cJSON_Parse(data);
        auto *cjson_groups = cJSON_GetObjectItem(in_json, "groups");
        assert(cJSON_IsArray(cjson_groups));
        auto array_size = cJSON_GetArraySize(cjson_groups);
        for (int i = 0; i < array_size; i++) {
            auto *item = cJSON_GetArrayItem(cjson_groups, i);
            RoomGroup in_rg;
            in_rg.from_json(item);
            groups.push_back(in_rg);
        }
        cJSON_Delete(in_json);
    }
};

void TileBatch::initialise_tiles_from_splash_screen(SplashScreenType splash_type) {
    // TODO: Move out parsing RoomGroup JSON (create 'Serialization' module)
    // *  TextureBank.cpp (Atlasc)
    // *  TextureRegion.cpp (Atlasc)
    // *  TileBrowserWindow.cpp (Tiles)
    // *  TileBatch.cpp (Tiles)

    // int temp[SPLASH_SCREEN_HEIGHT_TILES][SPLASH_SCREEN_WIDTH_TILES];
    std::vector<int> temp;
    int width, height;

    LevelEditorSave save;
    save.from_json(Assets::instance().get_ptr(Paths::Rooms::GROUPS));
    const RoomGroup &splash_screens = save.get_group("splash_screens");

    switch (splash_type) {
        case SplashScreenType::LEVEL_SUMMARY: {
            const Room &scores = splash_screens.get_room("level_summary_cave");
            width = scores.width_tiles;
            height = scores.height_tiles;
            temp.resize(scores.tiles.size());
            memcpy(temp.data(), scores.tiles.data(), sizeof(MapTileType) * scores.tiles.size());
            break;
        }
        case SplashScreenType::SCORES: {
            const Room &scores = splash_screens.get_room("scores");
            width = scores.width_tiles;
            height = scores.height_tiles;
            temp.resize(scores.tiles.size());
            memcpy(temp.data(), scores.tiles.data(), sizeof(MapTileType) * scores.tiles.size());
            break;
        }
        case SplashScreenType::MAIN_MENU: {
            const Room &main_menu = splash_screens.get_room("main_menu");
            width = main_menu.width_tiles;
            height = main_menu.height_tiles;
            temp.resize(main_menu.tiles.size());
            memcpy(temp.data(), main_menu.tiles.data(), sizeof(MapTileType) * main_menu.tiles.size());
            break;
        }
        case SplashScreenType::SANDBOX: {
            const Room &sandbox = splash_screens.get_room("sandbox");
            width = sandbox.width_tiles;
            height = sandbox.height_tiles;
            temp.resize(sandbox.tiles.size());
            memcpy(temp.data(), sandbox.tiles.data(), sizeof(MapTileType) * sandbox.tiles.size());
            break;
        }
        default: {
            assert(false);
        }
    }

    resize(width, height);

    // Now copy the specific splash screen 2D layout into the tile batch:
    for (int tab_y = 0; tab_y < height; tab_y++) {
        for (int tab_x = 0; tab_x < width; tab_x++) {
            at(tab_x, tab_y)->match_tile(static_cast<MapTileType>(temp.at(width * tab_y + tab_x)));
            at(tab_x, tab_y)->x = tab_x;
            at(tab_x, tab_y)->y = tab_y;
        }
    }
}

void TileBatch::initialise_tiles_from_room_layout(const RoomLayoutGenerator &room_layout) {
    _room_layout = room_layout;

    int temp[ROOM_WIDTH_TILES][ROOM_HEIGHT_TILES];

    LevelEditorSave save;
    save.from_json(Assets::instance().get_ptr(Paths::Rooms::GROUPS));
    const RoomGroup &shop_rooms_cave = save.get_group("shop_rooms_cave");
    const RoomGroup &entrance_rooms_cave = save.get_group("entrance_rooms_cave");
    const RoomGroup &exit_rooms_cave = save.get_group("exit_rooms_cave");
    const RoomGroup &other_cave = save.get_group("other_cave");
    const RoomGroup &closed_rooms_cave = save.get_group("closed_rooms_cave");
    const RoomGroup &left_right_rooms_cave = save.get_group("left_right_rooms_cave");
    const RoomGroup &left_right_down_rooms_cave = save.get_group("left_right_down_rooms_cave");
    const RoomGroup &left_right_up_rooms_cave = save.get_group("left_right_up_rooms_cave");

    // Iterate through every room there is:
    for (int room_y = ROOMS_COUNT_HEIGHT - 1; room_y >= 0; room_y--) {
        for (int room_x = 0; room_x < ROOMS_COUNT_WIDTH; room_x++) {
            switch (_room_layout.get_room_type_at_room(room_x, room_y)) {
                // Basing on the room type, randomly select a variation of this room
                // and copy it to the temporary array:
                case RoomType::CLOSED: {
                    const int possible_variations = closed_rooms_cave.rooms.size();
                    const int room_index = std::rand() % possible_variations;
                    _layout_room_ids[room_x][room_y] = room_index;
                    const Room &entrance = closed_rooms_cave.get_room(std::to_string(room_index).c_str());
                    memcpy(temp, entrance.tiles.data(), sizeof(MapTileType) * entrance.tiles.size());
                    break;
                }
                case RoomType::LEFT_RIGHT: {
                    const int possible_variations = left_right_rooms_cave.rooms.size();
                    const int room_index = std::rand() % possible_variations;
                    _layout_room_ids[room_x][room_y] = room_index;
                    const Room &entrance = left_right_rooms_cave.get_room(std::to_string(room_index).c_str());
                    memcpy(temp, entrance.tiles.data(), sizeof(MapTileType) * entrance.tiles.size());
                    break;
                }
                case RoomType::LEFT_RIGHT_DOWN: {
                    const int possible_variations = left_right_down_rooms_cave.rooms.size();
                    const int room_index = std::rand() % possible_variations;
                    _layout_room_ids[room_x][room_y] = room_index;
                    const Room &entrance = left_right_down_rooms_cave.get_room(std::to_string(room_index).c_str());
                    memcpy(temp, entrance.tiles.data(), sizeof(MapTileType) * entrance.tiles.size());
                    break;
                }
                case RoomType::LEFT_RIGHT_UP: {
                    const int possible_variations = left_right_up_rooms_cave.rooms.size();
                    const int room_index = std::rand() % possible_variations;
                    _layout_room_ids[room_x][room_y] = room_index;
                    const Room &entrance = left_right_up_rooms_cave.get_room(std::to_string(room_index).c_str());
                    memcpy(temp, entrance.tiles.data(), sizeof(MapTileType) * entrance.tiles.size());
                    break;
                }
                case RoomType::ENTRANCE: {
                    const int possible_variations = entrance_rooms_cave.rooms.size();
                    const int room_index = std::rand() % possible_variations;
                    _layout_room_ids[room_x][room_y] = room_index;
                    const Room &entrance = entrance_rooms_cave.get_room(std::to_string(room_index).c_str());
                    memcpy(temp, entrance.tiles.data(), sizeof(MapTileType) * entrance.tiles.size());
                    break;
                }
                case RoomType::EXIT: {
                    const int possible_variations = exit_rooms_cave.rooms.size();
                    const int room_index = std::rand() % possible_variations;
                    _layout_room_ids[room_x][room_y] = room_index;
                    const Room &entrance = exit_rooms_cave.get_room(std::to_string(room_index).c_str());
                    memcpy(temp, entrance.tiles.data(), sizeof(MapTileType) * entrance.tiles.size());
                    break;
                }
                // There are no room variations for the rooms below:
                case RoomType::SHOP_LEFT: {
                    const Room &shop = shop_rooms_cave.get_room("left");
                    memcpy(temp, shop.tiles.data(), sizeof(MapTileType) * shop.tiles.size());
                    break;
                }
                case RoomType::SHOP_RIGHT: {
                    const Room &shop = shop_rooms_cave.get_room("right");
                    memcpy(temp, shop.tiles.data(), sizeof(MapTileType) * shop.tiles.size());
                    break;
                }
                case RoomType::SHOP_LEFT_MUGSHOT: {
                    const Room &shop = shop_rooms_cave.get_room("mugshot_left");
                    memcpy(temp, shop.tiles.data(), sizeof(MapTileType) * shop.tiles.size());
                    break;
                }
                case RoomType::SHOP_RIGHT_MUGSHOT: {
                    const Room &shop = shop_rooms_cave.get_room("mugshot_right");
                    memcpy(temp, shop.tiles.data(), sizeof(MapTileType) * shop.tiles.size());
                    break;
                }
                case RoomType::ALTAR: {
                    const Room &altar = other_cave.get_room("altar");
                    memcpy(temp, altar.tiles.data(), sizeof(MapTileType) * altar.tiles.size());
                    break;
                }
                default: assert(false);
                    break;
            }

            for (int tab_y = 0; tab_y < ROOM_HEIGHT_TILES; tab_y++) {
                for (int tab_x = 0; tab_x < ROOM_WIDTH_TILES; tab_x++) {
                    // Offset accounting for the undestroyable tiles around the map and XY distance for the specific room:
                    int pos_x = static_cast<int>((2 + tab_x * 2 + 2 * ROOM_WIDTH_TILES * room_x) / 2);
                    int pos_y = static_cast<int>(
                        (2 + tab_y * 2 + 2 * ROOM_HEIGHT_TILES * ((ROOMS_COUNT_HEIGHT - room_y) - 1)) / 2);

                    at(pos_x, pos_y)->match_tile(static_cast<MapTileType>(temp[tab_y][tab_x]));
                    at(pos_x, pos_y)->x = pos_x;
                    at(pos_x, pos_y)->y = pos_y;
                }
            }
        }
    }
}

void TileBatch::get_first_tile_of_given_type(MapTileType map_tile_type, MapTile *&out) {
    for (int x = 0; x < _width_x_tiles; x++) {
        for (int y = 0; y < _height_y_tiles; y++) {
            if (at(x, y)->map_tile_type == map_tile_type) {
                out = at(x, y);
                return;
            }
        }
    }
}

TileBatch::TileBatch(int width_x_tiles, int height_y_tiles) : _width_x_tiles(width_x_tiles),
                                                              _height_y_tiles(height_y_tiles) {
    resize(_width_x_tiles, _height_y_tiles);
}

void TileBatch::resize(int width_x_tiles, int height_y_tiles) {
    _width_x_tiles = width_x_tiles;
    _height_y_tiles = height_y_tiles;
    _map_tiles.resize(width_x_tiles * height_y_tiles);
    for (int x = 0; x < width_x_tiles; x++) {
        for (int y = 0; y < height_y_tiles; y++) {
            auto* tile = at(x, y);
            tile->x = x;
            tile->y = y;
        }
    }
}

TileBatch::~TileBatch() {
}

void TileBatch::batch_vertices() {
    _mesh.clear();
    _indices.clear();

    std::size_t tile_counter = 0;

    for (int x = 0; x < _width_x_tiles; x++) {
        for (int y = 0; y < _height_y_tiles; y++) {
            MapTile *t = at(x, y);

            assert(at(x, y)->x == x);
            assert(at(x, y)->y == y);

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

entt::entity TileBatch::add_render_entity(entt::registry &registry) {
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

void TileBatch::get_neighbouring_tiles(float x, float y, MapTile *out_neighboring_tiles[9]) {
    std::uint16_t x_tiles = std::floor(x);
    std::uint16_t y_tiles = std::floor(y);

    assert(x_tiles < _width_x_tiles);
    assert(y_tiles < _height_y_tiles);

    MapTile *left_middle,
            *right_middle,
            *up_middle,
            *down_middle,
            *center,
            *left_up,
            *right_up,
            *left_down,
            *right_down;

    left_middle = x_tiles - 1 >= 0 ? at(x_tiles - 1, y_tiles) : nullptr;
    right_middle = x_tiles + 1 < _width_x_tiles ? at(x_tiles + 1, y_tiles) : nullptr;
    up_middle = y_tiles - 1 >= 0 ? at(x_tiles, y_tiles - 1) : nullptr;
    down_middle = y_tiles + 1 < _height_y_tiles ? at(x_tiles, y_tiles + 1) : nullptr;
    center = at(x_tiles, y_tiles);
    left_up = x_tiles - 1 >= 0 && y_tiles - 1 >= 0 ? at(x_tiles - 1, y_tiles - 1) : nullptr;
    right_up = x_tiles + 1 < _width_x_tiles && y_tiles - 1 >= 0 ? at(x_tiles + 1, y_tiles - 1) : nullptr;
    left_down = x_tiles - 1 >= 0 && y_tiles + 1 < _height_y_tiles ? at(x_tiles - 1, y_tiles + 1) : nullptr;
    right_down = x_tiles + 1 < _width_x_tiles && y_tiles + 1 < _height_y_tiles
                     ? at(x_tiles + 1, y_tiles + 1)
                     : nullptr;

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

void TileBatch::clean() {
    for (int x = 0; x < _width_x_tiles; x++) {
        for (int y = 0; y < _height_y_tiles; y++) {
            at(x, y)->match_tile(MapTileType::NOTHING);
            at(x, y)->destroyable = false;
            at(x, y)->x = x;
            at(x, y)->y = y;
        }
    }
}
