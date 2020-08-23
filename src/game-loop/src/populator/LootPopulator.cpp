#include "populator/LootPopulator.hpp"
#include "game-entities/GameEntity.hpp"
#include "game-entities/GoldBar.hpp"
#include "Level.hpp"
#include "logger/log.h"

#include <utility>

std::vector<std::shared_ptr<GameEntity>> populator::generate_loot()
{
    std::vector<std::shared_ptr<GameEntity>> out{};

    const auto& tile_batch = Level::instance().get_tile_batch();

    for (int tile_x = 0; tile_x < Consts::LEVEL_WIDTH_TILES; tile_x++)
    {
        for (int tile_y = 0; tile_y < Consts::LEVEL_HEIGHT_TILES; tile_y++)
        {
            const auto loot_type = tile_batch.get_loot_type_spawned_at(tile_x, tile_y);
            if (loot_type == LootType::GOLD_BAR)
            {
                out.push_back(std::make_shared<GoldBar>(tile_x + (MapTile::PHYSICAL_WIDTH / 2.0f), tile_y + (MapTile::PHYSICAL_HEIGHT / 2.0f)));
            }
        }
    }

    return std::move(out);
}
