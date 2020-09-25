#include "populator/Populator.hpp"
#include "populator/Spawner.hpp"
#include "game-entities/LevelSummaryTracker.hpp"
#include "game-entities/GameEntity.hpp"
#include "game-entities/SingleGoldBar.hpp"
#include "game-entities/TripleGoldBar.hpp"
#include "game-entities/BigGem.hpp"
#include "game-entities/Chest.hpp"
#include "game-entities/Jar.hpp"
#include "game-entities/Rock.hpp"
#include "Level.hpp"

#include <utility>
#include <random>

std::vector<std::shared_ptr<GameEntity>> populator::generate_loot(std::shared_ptr<LevelSummaryTracker>& tracker)
{
    Spawner single_gold_bar_spawner(6, 8);
    Spawner triple_gold_bar_spawner(3, 4);
    Spawner big_gem_spawner(3, 4);
    Spawner chest_spawner(3, 4);
    Spawner jar_spawner(3, 4);
    Spawner rock_spawner(2, 4);

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
                    if (rock_spawner.can_spawn())
                    {
                        rock_spawner.spawned();
                        out.push_back(std::make_shared<Rock>(pos_x, pos_y));
                    }
                    else if (jar_spawner.can_spawn())
                    {
                        jar_spawner.spawned();
                        out.push_back(std::make_shared<Jar>(pos_x, pos_y));
                    }
                    else if (chest_spawner.can_spawn())
                    {
                        chest_spawner.spawned();
                        out.push_back(std::make_shared<Chest>(pos_x, pos_y));
                    }
                    else if (triple_gold_bar_spawner.can_spawn())
                    {
                        triple_gold_bar_spawner.spawned();
                        auto triple_gold_bar = std::make_shared<TripleGoldBar>(pos_x, pos_y);
                        triple_gold_bar->add_observer(tracker.get());
                        out.push_back(triple_gold_bar);
                    }
                    else if (big_gem_spawner.can_spawn())
                    {
                        big_gem_spawner.spawned();
                        auto big_gem = std::make_shared<BigGem>(pos_x, pos_y);
                        big_gem->add_observer(tracker.get());
                        out.push_back(big_gem);
                    }
                    else if (single_gold_bar_spawner.can_spawn())
                    {
                        single_gold_bar_spawner.spawned();
                        auto single_gold_bar = std::make_shared<SingleGoldBar>(pos_x, pos_y);
                        single_gold_bar->add_observer(tracker.get());
                        out.push_back(single_gold_bar);
                    }
                    break;
                }
                default: assert(false);
            }
        }
    }

    return std::move(out);
}
