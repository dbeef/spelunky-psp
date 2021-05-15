#include "populator/Populator.hpp"
#include "populator/Spawner.hpp"
#include "populator/Shop.hpp"

#include "components/specialized/LevelSummaryTracker.hpp"
#include "components/generic/CollectibleComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/SaleableComponent.hpp"

#include "prefabs/collectibles/SingleGoldBar.hpp"
#include "prefabs/collectibles/TripleGoldBar.hpp"
#include "prefabs/collectibles/BigGem.hpp"
#include "prefabs/items/Rock.hpp"
#include "prefabs/items/Flare.hpp"
#include "prefabs/items/Jar.hpp"
#include "prefabs/items/Whip.hpp"
#include "prefabs/items/Crate.hpp"
#include "prefabs/items/Chest.hpp"
#include "prefabs/items/Arrow.hpp"
#include "prefabs/items/Rope.hpp"
#include "prefabs/items/Pistol.hpp"
#include "prefabs/items/BombSpawner.hpp"
#include "prefabs/items/RopeSpawner.hpp"
#include "prefabs/items/Bomb.hpp"
#include "prefabs/items/Cape.hpp"
#include "prefabs/items/SpringShoes.hpp"
#include "prefabs/items/SpikeShoes.hpp"
#include "prefabs/items/Mitt.hpp"
#include "prefabs/items/Glove.hpp"
#include "prefabs/items/Shotgun.hpp"
#include "prefabs/items/Skull.hpp"
#include "prefabs/items/Jetpack.hpp"
#include "prefabs/items/Compass.hpp"
#include "prefabs/items/GoldenIdol.hpp"
#include "prefabs/traps/Spikes.hpp"
#include "prefabs/traps/ArrowTrap.hpp"
#include "prefabs/npc/Snake.hpp"
#include "prefabs/npc/Spider.hpp"
#include "prefabs/npc/Bat.hpp"
#include "prefabs/npc/FakeSkeleton.hpp"
#include "prefabs/npc/Skeleton.hpp"
#include "prefabs/npc/Caveman.hpp"
#include "prefabs/npc/Shopkeeper.hpp"
#include "prefabs/npc/Damsel.hpp"

#include "EntityRegistry.hpp"
#include "Level.hpp"


void Populator::generate_inventory_items(entt::entity main_dude)
{
    auto& registry = EntityRegistry::instance().get_registry();

    auto& inventory = Inventory::instance();
    auto& dude_item_carrier_component = registry.get<ItemCarrierComponent>(main_dude);

    for (const auto& item_type : inventory.get_items())
    {
        entt::entity item = entt::null;

        switch (item_type)
        {
            case ItemType::ARROW: item = prefabs::Arrow::create(); break;
            case ItemType::BOMB: item = prefabs::Bomb::create(); break;
            case ItemType::CAPE: item = prefabs::Cape::create(); break;
            case ItemType::CHEST: item = prefabs::Chest::create(); break;
            case ItemType::CRATE: item = prefabs::Crate::create(); break;
            case ItemType::JAR: item = prefabs::Jar::create(); break;
            case ItemType::JETPACK: item = prefabs::Jetpack::create(); break;
            case ItemType::PISTOL: item = prefabs::Pistol::create(); break;
            case ItemType::ROCK: item = prefabs::Rock::create(); break;
            case ItemType::ROPE: item = prefabs::Rope::create(); break;
            case ItemType::SHOTGUN: item = prefabs::Shotgun::create(); break;
            case ItemType::SKULL: item = prefabs::Skull::create(); break;
            case ItemType::MITT: item = prefabs::Mitt::create(); break;
            case ItemType::GLOVE: item = prefabs::Glove::create(); break;
            case ItemType::SPIKE_SHOES: item = prefabs::SpikeShoes::create(); break;
            case ItemType::SPRING_SHOES: item = prefabs::SpringShoes::create(); break;
            case ItemType::COMPASS: item = prefabs::Compass::create(); break;
            case ItemType::FLARE: item = prefabs::Flare::create(); break;
            // These items are added by default when creating main dude instance:
            case ItemType::WHIP:
            case ItemType::BOMB_SPAWNER:
            case ItemType::ROPE_SPAWNER:
            // These items are not re-created:
            case ItemType::BODY:
            default: continue;
        }

        dude_item_carrier_component.pick_up_item(item, main_dude);
    }
}

void Populator::generate_npc(bool& damsel_rescued, bool shopkeeper_robbed)
{
    auto& registry = EntityRegistry::instance().get_registry();

    Spawner snake_spawner(3, 4);
    Spawner bat_spawner(3, 4);
    Spawner caveman_spawner(3, 4);
    Spawner fake_skeleton_spawner(3, 4);
    Spawner skeleton_spawner(3, 4);
    Spawner spikes_spawner(3, 4);
    Spawner spider_spawner(3, 3);
    Spawner damsel_spawner(1, 1);

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
                case NPCType::SHOPKEEPER:
                {
                    _shopkeepers.push_back(prefabs::Shopkeeper::create(pos_x, pos_y));
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
                        _npcs.push_back(entity);
                    }
                    else if (bat_spawner.can_spawn())
                    {
                        bat_spawner.spawned();
                        auto entity = prefabs::Bat::create(pos_x, pos_y);
                        _npcs.push_back(entity);
                    }
                    else if (caveman_spawner.can_spawn())
                    {
                        caveman_spawner.spawned();
                        auto entity = prefabs::Caveman::create(pos_x, pos_y);
                        _npcs.push_back(entity);
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
                        _npcs.push_back(entity);
                    }
                    else if (spikes_spawner.can_spawn())
                    {
                        spikes_spawner.spawned();
                        prefabs::Spikes::create(pos_x, pos_y);
                    }
                    else if (damsel_spawner.can_spawn())
                    {
                        damsel_spawner.spawned();
                        prefabs::Damsel::create(damsel_rescued, pos_x, pos_y);
                    }
                    break;
                }
                case NPCType::NOTHING: break;
                default: assert(false);
            }
        }
    }

    if (shopkeeper_robbed)
    {
        MapTile* map_tile = nullptr;
        tile_batch.get_first_tile_of_given_type(MapTileType::EXIT, map_tile);
        assert(map_tile);
        _shopkeepers.push_back(prefabs::Shopkeeper::create(map_tile->get_center_x(), map_tile->get_center_y()));
    }
}

void Populator::generate_loot(bool robbed)
{
    auto& registry = EntityRegistry::instance().get_registry();

    Spawner single_gold_bar_spawner(6, 8);
    Spawner triple_gold_bar_spawner(3, 4);
    Spawner big_gem_spawner(3, 4);
    Spawner chest_spawner(3, 4);
    Spawner jar_spawner(3, 4);
    Spawner rock_spawner(2, 12);
    Spawner crate_spawner(2, 4);

    Shop shop(robbed);

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
                case LootType::SHOP_ITEM:
                {
                    shop.make_next_item(pos_x, pos_y);
                    break;
                }
                case LootType::GOLDEN_IDOL:
                {
                    // Offset coming from the fact that layout arrays can't define item spawn position in-between tiles.
                    const auto offset = (MapTile::PHYSICAL_WIDTH / 2.0f);
                    prefabs::GoldenIdol::create(pos_x + offset, pos_y);
                    break;
                }
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
                        _collectibles.push_back(entity);
                    }
                    else if (big_gem_spawner.can_spawn())
                    {
                        big_gem_spawner.spawned();
                        auto entity = prefabs::BigGem::create(pos_x, pos_y);
                        _collectibles.push_back(entity);
                    }
                    else if (single_gold_bar_spawner.can_spawn())
                    {
                        single_gold_bar_spawner.spawned();
                        auto entity = prefabs::SingleGoldBar::create(pos_x, pos_y);
                        _collectibles.push_back(entity);
                    }
                    break;
                }
                default: assert(false);
            }
        }
    }
}
