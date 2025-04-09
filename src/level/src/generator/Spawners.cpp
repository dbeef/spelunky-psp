#include "generator/Spawners.hpp"
#include "populator/Spawner.hpp"
#include "TileBatch.hpp"

#include "prefabs/collectibles/BigGem.hpp"
#include "prefabs/collectibles/SingleGoldBar.hpp"
#include "prefabs/items/Chest.hpp"
#include "prefabs/items/Crate.hpp"
#include "prefabs/items/GoldenIdol.hpp"
#include "prefabs/items/Jar.hpp"
#include "prefabs/items/Rock.hpp"
#include "prefabs/npc/Bat.hpp"
#include "prefabs/npc/Caveman.hpp"
#include "prefabs/npc/Damsel.hpp"
#include "prefabs/npc/FakeSkeleton.hpp"
#include "prefabs/npc/Shopkeeper.hpp"
#include "prefabs/npc/Skeleton.hpp"
#include "prefabs/npc/Snake.hpp"
#include "prefabs/npc/Spider.hpp"
#include "prefabs/traps/ArrowTrap.hpp"
#include "prefabs/traps/Spikes.hpp"

#include <vector>

#include "LevelType.hpp"
#include "logger/log.h"
#include "prefabs/collectibles/TripleGoldBar.hpp"

namespace {
    constexpr int operator"" _max(unsigned long long value) {
        return static_cast<int>(value);
    }

    constexpr int operator"" _percent(unsigned long long value) {
        if (value > 100) {
            throw std::out_of_range("Percentage must be between 0 and 100");
        }
        return static_cast<int>(value);
    }

    namespace Predicates {
        // TODO: Helper for joining predicates with && operator; make loot other than the golden idol not spawn on altar

        const auto has_tile_below = [ ](const TileBatch &tile_batch, const MapTile *current_tile) -> bool {
            const auto *below = tile_batch.below(current_tile);
            return below && below->collidable;
        };

        const auto altar_below = [](const TileBatch &tile_batch, const MapTile *current_tile) -> bool {
            const auto *tile_below = tile_batch.below(current_tile);
            return tile_below && tile_below->map_tile_type == MapTileType::ALTAR_LEFT;
        };

        const auto has_tile_above_and_no_tile_below = [ ](const TileBatch &tile_batch,
                                                          const MapTile *current_tile) -> bool {
            const auto *above = tile_batch.above(current_tile);
            const auto *below = tile_batch.below(current_tile);
            return above && above->collidable && below && !below->collidable;
        };

        const auto on_arrow_trap_left_tile = [ ](const TileBatch &tile_batch, const MapTile *current_tile) -> bool {
            return current_tile->map_tile_type == MapTileType::ARROW_TRAP_LEFT;
        };

        const auto on_arrow_trap_right_tile = [ ](const TileBatch &tile_batch, const MapTile *current_tile) -> bool {
            return current_tile->map_tile_type == MapTileType::ARROW_TRAP_RIGHT;
        };
    }

    // TODO: Events that something is spawned should go directly from prefabs (i.e loot spawned)
    // TODO: Config file for externally modifying chance of spawn without the need for recompilation
    // TODO: Move to Spawners.hpp / Spawners.cpp
    std::vector<Spawner> get_cave_loot_spawners() {
        return std::vector{
            Spawner{
                6_percent, 8_max,
                [](const MapTile *map_tile) {
                    const auto [x, y] = map_tile->center();
                    return prefabs::SingleGoldBar::create(x, y);
                },
                Predicates::has_tile_below
            },
            Spawner{
                4_percent, 4_max,
                [](const MapTile *map_tile) {
                    const auto [x, y] = map_tile->center();
                    return prefabs::TripleGoldBar::create(x, y);
                },
                Predicates::has_tile_below
            },
            Spawner{
                3_percent, 4_max,
                [](const MapTile *map_tile) {
                    const auto [x, y] = map_tile->center();
                    return prefabs::BigGem::create(x, y);
                },
                Predicates::has_tile_below
            },
            Spawner{
                3_percent, 4_max,
                [](const MapTile *map_tile) {
                    const auto [x, y] = map_tile->center();
                    return prefabs::Chest::create(x, y);
                },
                Predicates::has_tile_below
            },
            Spawner{
                3_percent, 4_max,
                [](const MapTile *map_tile) {
                    const auto [x, y] = map_tile->center();
                    return prefabs::Jar::create(x, y);
                },
                Predicates::has_tile_below
            },
            Spawner{
                2_percent, 12_max,
                [](const MapTile *map_tile) {
                    const auto [x, y] = map_tile->center();
                    return prefabs::Rock::create(x, y);
                },
                Predicates::has_tile_below
            },
            Spawner{
                2_percent, 4_max,
                [](const MapTile *map_tile) {
                    const auto [x, y] = map_tile->center();
                    return prefabs::Crate::create(x, y);
                },
                Predicates::has_tile_below
            },
            Spawner{
                100_percent, 1_max,
                [](const MapTile *map_tile) {
                    const auto [x, y] = map_tile->center();
                    return prefabs::GoldenIdol::create(x + (MapTile::PHYSICAL_WIDTH / 2.0f), y);
                },
                Predicates::altar_below
            },
        };
    }

    std::vector<Spawner> get_cave_npc_spawners() {
        return std::vector{
            Spawner{
                3_percent, 4_max,
                [](const MapTile *map_tile) {
                    const auto [x, y] = map_tile->center();
                    return prefabs::Snake::create(x, y);
                },
                Predicates::has_tile_above_and_no_tile_below
            },
            Spawner{
                3_percent, 4_max,
                [](const MapTile *map_tile) {
                    const auto [x, y] = map_tile->center();
                    return prefabs::Caveman::create(x, y);
                },
                Predicates::has_tile_below
            },
            Spawner{
                3_percent, 4_max,
                [](const MapTile *map_tile) {
                    const auto [x, y] = map_tile->center();
                    return prefabs::Skeleton::create(x, y);
                },
                Predicates::has_tile_below
            },
            Spawner{
                3_percent, 4_max,
                [](const MapTile *map_tile) {
                    const auto [x, y] = map_tile->center();
                    return prefabs::Spikes::create(x, y);
                },
                Predicates::has_tile_below
            },
            Spawner{
                3_percent, 3_max,
                [](const MapTile *map_tile) {
                    const auto [x, y] = map_tile->center();
                    return prefabs::Spider::create(x, y);
                },
                Predicates::has_tile_above_and_no_tile_below
            },
            Spawner{
                3_percent, 3_max,
                [](const MapTile *map_tile) {
                    const auto [x, y] = map_tile->center();
                    return prefabs::Bat::create(x, y);
                },
                Predicates::has_tile_above_and_no_tile_below
            },
            Spawner{
                1_percent, 1_max,
                [](const MapTile *map_tile) {
                    const auto [x, y] = map_tile->center();
                    static bool damsel_rescued = false; // FIXME: Reimplement this functionality with events
                    return prefabs::Damsel::create(damsel_rescued, x, y);
                },
                Predicates::has_tile_below
            },
            Spawner{
                3_percent, 4_max,
                [](const MapTile *map_tile) {
                    const auto [x, y] = map_tile->center();
                    prefabs::FakeSkeleton::create(x, y);
                    return entt::null; // Since fake skeleton is composed of two separate skull and bones' entities.
                },
                Predicates::has_tile_below
            },
            Spawner{
                100_percent, Spawner::UNLIMITED,
                [](const MapTile *map_tile) {
                    const auto [x, y] = map_tile->center();
                    log_info("Found an arrow trap at: %f %f", x, y);
                    return prefabs::ArrowTrap::create(x, y, HorizontalOrientation::LEFT);
                },
                Predicates::on_arrow_trap_left_tile
            },
            Spawner{
                100_percent, Spawner::UNLIMITED,
                [](const MapTile *map_tile) {
                    const auto [x, y] = map_tile->center();
                    return prefabs::ArrowTrap::create(x, y, HorizontalOrientation::RIGHT);
                },
                Predicates::on_arrow_trap_right_tile
            },
        };
    }
}

namespace Spawners {
    std::vector<Spawner> get_npc_spawners(LevelType level_type) {
        switch (level_type) {
            case LevelType::CAVE: return get_cave_npc_spawners();
            case LevelType::MAIN_MENU:
            case LevelType::JUNGLE:
            case LevelType::ICE:
            case LevelType::LAVA:
            case LevelType::BOSS:
            default: assert(false);
                return {};
        }
    }

    std::vector<Spawner> get_loot_spawners(LevelType level_type) {
        switch (level_type) {
            case LevelType::CAVE: return get_cave_loot_spawners();
            case LevelType::MAIN_MENU:
            case LevelType::JUNGLE:
            case LevelType::ICE:
            case LevelType::LAVA:
            case LevelType::BOSS:
            default: assert(false);
                return {};
        }
    }
}
