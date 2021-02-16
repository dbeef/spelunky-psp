#include "populator/Populator.hpp"
#include "populator/Spawner.hpp"

#include "components/specialized/LevelSummaryTracker.hpp"
#include "components/generic/CollectibleComponent.hpp"

#include "prefabs/collectibles/SingleGoldBar.hpp"
#include "prefabs/collectibles/TripleGoldBar.hpp"
#include "prefabs/collectibles/BigGem.hpp"
#include "prefabs/items/Rock.hpp"
#include "prefabs/items/Jar.hpp"
#include "prefabs/items/Crate.hpp"
#include "prefabs/items/Chest.hpp"
#include "prefabs/traps/Spikes.hpp"
#include "prefabs/traps/ArrowTrap.hpp"
#include "prefabs/npc/Snake.hpp"
#include "prefabs/npc/Spider.hpp"
#include "prefabs/npc/Bat.hpp"
#include "prefabs/npc/FakeSkeleton.hpp"
#include "prefabs/npc/Skeleton.hpp"
#include "prefabs/npc/Caveman.hpp"

#include "EntityRegistry.hpp"
#include "Level.hpp"

void populator::generate_npc(std::shared_ptr<LevelSummaryTracker>& tracker)
{
    auto& registry = EntityRegistry::instance().get_registry();

//    Spawner snake_spawner(10, 20);
//    Spawner bat_spawner(3, 0);
//    Spawner caveman_spawner(3, 0);
//    Spawner fake_skeleton_spawner(3, 0);
//    Spawner skeleton_spawner(3, 0);
//    Spawner spikes_spawner(3, 0);
//    Spawner spider_spawner(3, 0);

    Spawner snake_spawner(3, 4);
    Spawner bat_spawner(3, 4);
    Spawner caveman_spawner(3, 4);
    Spawner fake_skeleton_spawner(3, 4);
    Spawner skeleton_spawner(3, 4);
    Spawner spikes_spawner(3, 4);
    Spawner spider_spawner(3, 3);

    std::vector<std::shared_ptr<GameEntity>> out{};

    const auto& tile_batch = Level::instance().get_tile_batch();

    for (int tile_x = 0; tile_x < Consts::LEVEL_WIDTH_TILES; tile_x++)
    {
        for (int tile_y = 0; tile_y < Consts::LEVEL_HEIGHT_TILES; tile_y++)
        {
            const auto npc_type = tile_batch.get_npc_type_spawned_at(tile_x, tile_y);

            float pos_x = static_cast<float>(tile_x) + (MapTile::PHYSICAL_WIDTH / 2.0f);
            float pos_y = static_cast<float>(tile_y) + (MapTile::PHYSICAL_HEIGHT / 2.0f);

            switch (npc_type)
            {
                case NPCType::SPIDER:
                {
                    if (spider_spawner.can_spawn())
                    {
                        spider_spawner.spawned();
                        prefabs::Spider::create(pos_x, pos_y);

                    }
                    break;
                }
                case NPCType::ARROW_TRAP_LEFT:
                {
                    prefabs::ArrowTrap::create(pos_x, pos_y, HorizontalOrientation::LEFT);
                    break;
                }
                case NPCType::ARROW_TRAP_RIGHT:
                {
                    prefabs::ArrowTrap::create(pos_x, pos_y, HorizontalOrientation::RIGHT);
                    break;
                }
                case NPCType::ANY:
                {
                    if (snake_spawner.can_spawn())
                    {
                        snake_spawner.spawned();
                        auto entity = prefabs::Snake::create(pos_x, pos_y);
                        auto& hitpoints = registry.get<HitpointComponent>(entity);
                        hitpoints.add_observer(tracker.get());
                    }
                    else if (bat_spawner.can_spawn())
                    {
                        bat_spawner.spawned();
                        auto entity = prefabs::Bat::create(pos_x, pos_y);
                        auto& hitpoints = registry.get<HitpointComponent>(entity);
                        hitpoints.add_observer(tracker.get());
                    }
                    else if (caveman_spawner.can_spawn())
                    {
                        caveman_spawner.spawned();
                        auto entity = prefabs::Caveman::create(pos_x, pos_y);
                        auto& hitpoints = registry.get<HitpointComponent>(entity);
                        hitpoints.add_observer(tracker.get());
                    }
                    else if (fake_skeleton_spawner.can_spawn())
                    {
                        fake_skeleton_spawner.spawned();
                        prefabs::FakeSkeleton::create(pos_x, pos_y);
                    }
                    else if (skeleton_spawner.can_spawn())
                    {
                        skeleton_spawner.spawned();
                        auto entity = prefabs::Skeleton::create(pos_x, pos_y);
                        auto& hitpoints = registry.get<HitpointComponent>(entity);
                        hitpoints.add_observer(tracker.get());
                    }
                    else if (spikes_spawner.can_spawn())
                    {
                        spikes_spawner.spawned();
                        prefabs::Spikes::create(pos_x, pos_y);
                    }
                    break;
                }
                case NPCType::NOTHING: break;
                default: assert(false);
            }
        }
    }
}

void populator::generate_loot(std::shared_ptr<LevelSummaryTracker>& tracker)
{
    auto& registry = EntityRegistry::instance().get_registry();

    Spawner single_gold_bar_spawner(6, 8);
    Spawner triple_gold_bar_spawner(3, 4);
    Spawner big_gem_spawner(3, 4);
    Spawner chest_spawner(3, 4);
    Spawner jar_spawner(3, 4);
    Spawner rock_spawner(2, 12);
    Spawner crate_spawner(2, 4);

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
                    else if (crate_spawner.can_spawn())
                    {
                        crate_spawner.spawned();
                        prefabs::Crate::create(pos_x, pos_y);
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
