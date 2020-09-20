#include "populator/Populator.hpp"
#include "populator/Spawner.hpp"
#include "game-entities/GameEntity.hpp"
#include "game-entities/SingleGoldBar.hpp"
#include "game-entities/TripleGoldBar.hpp"
#include "game-entities/BigGem.hpp"
#include "Level.hpp"

#include <utility>
#include <random>

std::vector<std::shared_ptr<GameEntity>> populator::generate_loot()
{
    Spawner<SingleGoldBar> single_gold_bar_spawner(6, 8);
    Spawner<TripleGoldBar> triple_gold_bar_spawner(3, 4);
    Spawner<BigGem> big_gem_spawner(3, 4);

    std::vector<std::shared_ptr<GameEntity>> out{};

    const auto& tile_batch = Level::instance().get_tile_batch();

    for (int tile_x = 0; tile_x < Consts::LEVEL_WIDTH_TILES; tile_x++)
    {
        for (int tile_y = 0; tile_y < Consts::LEVEL_HEIGHT_TILES; tile_y++)
        {
            const auto loot_type = tile_batch.get_loot_type_spawned_at(tile_x, tile_y);

            float pos_x = static_cast<float>(tile_x) + (MapTile::PHYSICAL_WIDTH / 2.0f);
            float pos_y = static_cast<float>(tile_y) + (MapTile::PHYSICAL_HEIGHT / 2.0f);

            switch (loot_type)
            {
                case LootType::NOTHING: break;
                case LootType::ANY:
                {
                    if (triple_gold_bar_spawner.can_spawn())
                    {
                        out.push_back(triple_gold_bar_spawner.spawn(pos_x, pos_y));
                    }
                    else if (big_gem_spawner.can_spawn())
                    {
                        out.push_back(big_gem_spawner.spawn(pos_x, pos_y));
                    }
                    else if (single_gold_bar_spawner.can_spawn())
                    {
                        out.push_back(single_gold_bar_spawner.spawn(pos_x, pos_y));
                    }
                    break;
                }
                default: assert(false);
            }
        }
    }

    return std::move(out);
}
