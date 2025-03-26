#pragma once

#include "Vertex.hpp"
#include "IndexType.hpp"

#include "MapTile.hpp"
#include "SplashScreenType.hpp"
#include "RoomType.hpp"
#include "LootType.hpp"
#include "NPCType.hpp"

#include <vector>
#include <entt/entt.hpp>

#include "RoomLayoutGenerator.hpp"


// TODO: Rework TileBatch so it only holds tiles and adds some convenience methods to access/modify them.
//       Move level-generation functions (i.e place an anltar) to the relevant pipeline.
class TileBatch {

public:

    struct LevelGeneratorParams
    {
        bool shopkeeper_robbed = false;
    };

    explicit TileBatch(int width_x_tiles, int width_y_tiles);

    void resize(int width_x_tiles, int width_y_tiles);

    ~TileBatch();

    void initialise_tiles_from_room_layout(const RoomLayoutGenerator& room_layout);

    void initialise_tiles_from_splash_screen(SplashScreenType splashScreenType);

    void get_first_tile_of_given_type(MapTileType map_tile_type, MapTile *&out);

    void batch_vertices();

    const RoomLayoutGenerator& get_room_layout() const { return _room_layout; }

    // TODO: Use optional
    MapTile* above(const MapTile* tile) {
        if (tile->y - 1 < 0) {
            return nullptr;
        }

        return at(tile->x, tile->y - 1);
    }

    const MapTile* above(const MapTile* tile) const {
        if (tile->y - 1 < 0) {
            return nullptr;
        }

        return at(tile->x, tile->y - 1);
    }

    MapTile* at(int tile_x, int tile_y) {
        const std::size_t index = (tile_y * _width_x_tiles) + tile_x;
        return &_map_tiles.at(index);
    }

    const MapTile* at(int tile_x, int tile_y) const {
        const std::size_t index = (tile_y * _width_x_tiles) + tile_x;
        return &_map_tiles.at(index);
    }

    MapTile* below(const MapTile* tile) {
        if (tile->y + 1 >= _height_y_tiles) {
            return nullptr;
        }

        return at(tile->x, tile->y + 1);
    }

    const MapTile* below(const MapTile* tile) const {
        if (tile->y + 1 >= _height_y_tiles) {
            return nullptr;
        }

        return at(tile->x, tile->y + 1);
    }

    entt::entity add_render_entity(entt::registry &registry);

    void get_neighbouring_tiles(float x, float y, MapTile *out_neighboring_tiles[9]);

    void clean();

    int get_width_tiles() const { return _width_x_tiles; }

    int get_height_tiles() const { return _height_y_tiles; }

private:

    std::vector<MapTile> _map_tiles;
    // MapTile *map_tiles[Consts::LEVEL_WIDTH_TILES][Consts::LEVEL_HEIGHT_TILES]{};

    int _width_x_tiles = 0;
    int _height_y_tiles = 0;

    // Any encountered closed room will be turned into an altar.
    void place_an_altar();

    // Finds a closed room that is not blocked from either left or right side by other closed room,
    // and plants a shop there that is oriented to the not-blocked side.
    void place_a_shop(bool shopkeeper_robbed);

    std::vector<Vertex> _mesh;
    std::vector<IndexType> _indices;

    RoomLayoutGenerator _room_layout;
    int _layout_room_ids[Consts::ROOMS_COUNT_WIDTH][Consts::ROOMS_COUNT_HEIGHT]{};
};
