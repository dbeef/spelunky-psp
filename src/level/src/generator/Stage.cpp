#include "Stage.hpp"

#include <random>

#include "Level.hpp"
#include "MapTile.hpp"
#include "MapTileType.hpp"
#include "TileBatch.hpp"
#include "generator/Spawners.hpp"
#include "logger/log.h"
#include "populator/Spawner.hpp"


namespace Stage {
    template<>
    void run<Type::GENERATE_BACKGROUND>(TileBatch &output_batch, EntityRegistry &, LevelType) {
        const std::uint16_t width_tiles = output_batch.get_width_tiles();
        const std::uint16_t height_tiles = output_batch.get_height_tiles();

        for (int x = 0; x < width_tiles; x++) {
            for (int y = 0; y < height_tiles; y++) {
                MapTile* current = output_batch.at(x, y);
                if (current->map_tile_type == MapTileType::NOTHING) {
                    // Background consists of 4 tiles (CAVE_BG_1 to CAVE_BG_4) placed on a 2x2 grid:
                    const int type_index = (x % 2) + ((y % 2) * 2);
                    const auto type = static_cast<MapTileType>(type_index + static_cast<int>(MapTileType::CAVE_BG_1));
                    current->map_tile_type = type;
                    current->match_tile(type);
                }
            }
        }
    }

    template<>
    void run<Type::POST_PROCESS_TILES>(TileBatch &output_batch, EntityRegistry &output_registry, LevelType level_type) {
        for (int tile_x = 0; tile_x < output_batch.get_width_tiles(); tile_x++) {
            for (int tile_y = 0; tile_y < output_batch.get_height_tiles(); tile_y++) {
                auto *current = output_batch.at(tile_x, tile_y);
                auto* above = output_batch.above(current);
                auto* below = output_batch.below(current);

                switch (current->map_tile_type) {
                    case MapTileType::CAVE_REGULAR:
                    case MapTileType::CAVE_UP_ORIENTED:
                    case MapTileType::CAVE_DOWN_ORIENTED:
                    case MapTileType::CAVE_UP_DOWN_ORIENTED: {
                        if (above && above->collidable && below && !below->collidable) {
                            current->map_tile_type = MapTileType::CAVE_DOWN_ORIENTED;
                        } else if (above && !above->collidable && below && below->collidable) {
                            current->map_tile_type = MapTileType::CAVE_UP_ORIENTED;
                        } else if (above && above->collidable && below && below->collidable) {
                            current->map_tile_type = MapTileType::CAVE_REGULAR;
                        } else if (above && !above->collidable && below && !below->collidable) {
                            current->map_tile_type = MapTileType::CAVE_UP_DOWN_ORIENTED;
                        } else if (above && !above->collidable && !below) {
                            current->map_tile_type = MapTileType::CAVE_UP_ORIENTED;
                        }
                        break;
                    }
                    default: ;
                }
            }
        }
    }

    template<>
    void run<Type::GENERATE_NPC>(TileBatch &output_batch, EntityRegistry &output_registry, LevelType level_type) {
        std::random_device random_device;
        std::default_random_engine engine(random_device());

        auto spawners = Spawners::get_npc_spawners(level_type);
        std::shuffle(spawners.begin(), spawners.end(), engine);

        auto &tile_batch = Level::instance().get_tile_batch();
        const auto& room_layout = tile_batch.get_room_layout();

        for (int tile_x = 0; tile_x < tile_batch.get_width_tiles(); tile_x++) {
            for (int tile_y = 0; tile_y < tile_batch.get_height_tiles(); tile_y++) {
                auto *map_tile = tile_batch.at(tile_x, tile_y);

                const auto room_type = room_layout.get_room_type_at_tile(tile_x, tile_y);

                if (room_type == RoomType::ENTRANCE ||
                    room_type == RoomType::ALTAR ||
                    room_type == RoomType::SHOP_RIGHT ||
                    room_type == RoomType::SHOP_LEFT ||
                    room_type == RoomType::SHOP_LEFT_MUGSHOT ||
                    room_type == RoomType::SHOP_RIGHT_MUGSHOT ) {
                    log_info("Ignoring room at: %i %i, type: %i", tile_x, tile_y, static_cast<int>(room_type));
                    // FIXME: Detection fails for some reason!
                    continue;
                }

                for (auto &spawner: spawners) {
                    if (map_tile->collidable &&
                        map_tile->map_tile_type != MapTileType::ARROW_TRAP_LEFT &&
                        map_tile->map_tile_type != MapTileType::ARROW_TRAP_RIGHT ) {
                        continue; // As there is no type of NPC spawning directly on a collidable tile
                    }
                    if (spawner.try_spawn(engine, tile_batch, map_tile)) {
                        break;
                    }
                }
            }
        }
    }

    template<>
    void run<Type::GENERATE_LOOT>(TileBatch &output_batch, EntityRegistry &output_registry, LevelType level_type) {
        std::random_device random_device;
        std::default_random_engine engine(random_device());

        auto spawners = Spawners::get_loot_spawners(level_type);
        auto &tile_batch = Level::instance().get_tile_batch();

        for (int tile_x = 0; tile_x < tile_batch.get_width_tiles(); tile_x++) {
            for (int tile_y = 0; tile_y < tile_batch.get_height_tiles(); tile_y++) {
                auto *map_tile = tile_batch.at(tile_x, tile_y);

                for (auto &spawner: spawners) {
                    if (map_tile->collidable) {
                        continue;
                    }
                    if (spawner.try_spawn(engine, tile_batch, map_tile)) {
                        break;
                    }
                }
            }
        }
    }

    template<>
    void run<Type::GENERATE_INDESTRUCTIBLE_FRAME>(TileBatch &output_batch, EntityRegistry &, LevelType level_type) {
        const int width_tiles = output_batch.get_width_tiles();
        const int height_tiles = output_batch.get_height_tiles();

        const std::array ranges = {
            std::make_pair(std::make_pair(0, width_tiles), std::make_pair(0, 1)),
            std::make_pair(std::make_pair(0, 1), std::make_pair(0, height_tiles)),
            std::make_pair(std::make_pair(width_tiles - 1, width_tiles), std::make_pair(0, height_tiles)),
            std::make_pair(std::make_pair(0, width_tiles), std::make_pair(height_tiles - 1, height_tiles)),
        };

        for (auto &range: ranges) {
            const auto &x_range = range.first;
            const auto &y_range = range.second;

            for (int x = x_range.first; x < x_range.second; x++) {
                for (int y = y_range.first; y < y_range.second; y++) {
                    MapTile* tile = output_batch.at(x, y);
                    tile->match_tile(MapTileType::CAVE_REGULAR);
                    tile->destroyable = false;
                }
            }
        }
    }
}
