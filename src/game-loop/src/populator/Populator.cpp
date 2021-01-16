#include "EntityRegistry.hpp"
#include "populator/Populator.hpp"
#include "populator/Spawner.hpp"
#include "components/specialized/LevelSummaryTracker.hpp"

#include "prefabs/collectibles/SingleGoldBar.hpp"
#include "prefabs/collectibles/TripleGoldBar.hpp"
#include "prefabs/collectibles/BigGem.hpp"
#include "prefabs/items/Rock.hpp"
#include "prefabs/items/Jar.hpp"
#include "prefabs/items/Chest.hpp"
#include "Level.hpp"

#include <utility>
#include <random>
#include <components/generic/CollectibleComponent.hpp>

void populator::generate_loot(std::shared_ptr<LevelSummaryTracker>& tracker)
{
    auto& registry = EntityRegistry::instance().get_registry();

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
                        prefabs::Rock::create(pos_x, pos_y);
                    }
                    else if (jar_spawner.can_spawn())
                    {
                        jar_spawner.spawned();
                        prefabs::Jar::create(pos_x, pos_y);
                    }
                    else if (chest_spawner.can_spawn())
                    {
                        chest_spawner.spawned();
                        prefabs::Chest::create(pos_x, pos_y);
                    }
                    else if (triple_gold_bar_spawner.can_spawn())
                    {
                        triple_gold_bar_spawner.spawned();
                        auto entity = prefabs::TripleGoldBar::create(pos_x, pos_y);
                        auto& collectible = registry.get<CollectibleComponent>(entity);
                        collectible.add_observer(tracker.get());
                    }
                    else if (big_gem_spawner.can_spawn())
                    {
                        big_gem_spawner.spawned();
                        auto entity = prefabs::BigGem::create(pos_x, pos_y);
                        auto& collectible = registry.get<CollectibleComponent>(entity);
                        collectible.add_observer(tracker.get());
                    }
                    else if (single_gold_bar_spawner.can_spawn())
                    {
                        single_gold_bar_spawner.spawned();
                        auto entity = prefabs::SingleGoldBar::create(pos_x, pos_y);
                        auto& collectible = registry.get<CollectibleComponent>(entity);
                        collectible.add_observer(tracker.get());
                    }
                    break;
                }
                default: assert(false);
            }
        }
    }
}
