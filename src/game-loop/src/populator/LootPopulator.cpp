#include "populator/LootPopulator.hpp"
#include "game-entities/GameEntity.hpp"
#include "game-entities/GoldBar.hpp"
#include "Level.hpp"

#include <utility>
#include <random>

namespace
{
    int get_random_number()
    {
        static std::random_device random_device;
        std::default_random_engine engine(random_device());
        std::uniform_int_distribution<int> uniform_dist(0, 10);
        return uniform_dist(engine);
    }

    const std::uint16_t MAX_GOLD_BARS_IN_LEVEL = 10;
}

std::vector<std::shared_ptr<GameEntity>> populator::generate_loot()
{
    std::vector<std::shared_ptr<GameEntity>> out{};
    std::uint16_t gold_bars_spawned = 0;

    const auto& tile_batch = Level::instance().get_tile_batch();

    for (int tile_x = 0; tile_x < Consts::LEVEL_WIDTH_TILES; tile_x++)
    {
        for (int tile_y = 0; tile_y < Consts::LEVEL_HEIGHT_TILES; tile_y++)
        {
            const auto loot_type = tile_batch.get_loot_type_spawned_at(tile_x, tile_y);
            bool spawned = (get_random_number() % 8) == 0;

            if (spawned)
            {
                if (loot_type == LootType::GOLD_BAR && gold_bars_spawned < MAX_GOLD_BARS_IN_LEVEL)
                {
                    out.push_back(std::make_shared<GoldBar>(tile_x + (MapTile::PHYSICAL_WIDTH / 2.0f),
                                                            tile_y + (MapTile::PHYSICAL_HEIGHT / 2.0f)));
                    gold_bars_spawned++;
                }
            }
        }
    }

    return std::move(out);
}
