#include "populator/Shop.hpp"
#include "EntityRegistry.hpp"
#include "Level.hpp"
#include "components/generic/SaleableComponent.hpp"

#include "prefabs/items/Rock.hpp"
#include "prefabs/items/Wallet.hpp"
#include "prefabs/items/Jar.hpp"
#include "prefabs/items/Whip.hpp"
#include "prefabs/items/Crate.hpp"
#include "prefabs/items/Chest.hpp"
#include "prefabs/items/Arrow.hpp"
#include "prefabs/items/BombBag.hpp"
#include "prefabs/items/RopePile.hpp"
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

#include <algorithm>
#include <random>

namespace
{
    enum class ShopTheme
    {
        WEAPONS,
        CLOTHES,
        _SIZE
    };

    ShopTheme get_random_theme()
    {
        return static_cast<ShopTheme>(std::rand() % static_cast<int>(ShopTheme::_SIZE));
    }

    // TODO: This helper could be moved out to populator namespace and also applied in Crate/populator::generate_inventory_items
    entt::entity make_item(ItemType item_type)
    {
        switch(item_type)
        {
            case ItemType::ARROW: return prefabs::Arrow::create();
            case ItemType::BOMB: return prefabs::Bomb::create();
            case ItemType::CAPE: return prefabs::Cape::create();
            case ItemType::CHEST: return prefabs::Chest::create();
            case ItemType::CRATE: return prefabs::Crate::create();
            case ItemType::JAR: return prefabs::Jar::create();
            case ItemType::JETPACK: return prefabs::Jetpack::create();
            case ItemType::PISTOL: return prefabs::Pistol::create();
            case ItemType::ROCK: return prefabs::Rock::create();
            case ItemType::ROPE: return prefabs::Rope::create();
            case ItemType::SHOTGUN: return prefabs::Shotgun::create();
            case ItemType::SKULL: return prefabs::Skull::create();
            case ItemType::WHIP: return prefabs::Whip::create();
            case ItemType::BOMB_SPAWNER: return prefabs::BombSpawner::create(0);
            case ItemType::ROPE_SPAWNER: return prefabs::RopeSpawner::create(0);
            case ItemType::WALLET: return prefabs::Wallet::create();
            case ItemType::SPIKE_SHOES: return prefabs::SpikeShoes::create();
            case ItemType::SPRING_SHOES: return prefabs::SpringShoes::create();
            case ItemType::MITT: return prefabs::Mitt::create();
            case ItemType::GLOVE: return prefabs::Glove::create();
            case ItemType::COMPASS: return prefabs::Compass::create();
            case ItemType::BOMB_BAG: return prefabs::BombBag::create();
            case ItemType::ROPE_PILE: return prefabs::RopePile::create();
            case ItemType::_SIZE:
            default: assert(false);
        }
        return entt::null;
    }

    void set_position(entt::entity item, float x_center, float y_center)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        if (registry.has<PositionComponent>(item))
        {
            auto& position = registry.get<PositionComponent>(item);
            position.x_center = x_center;
            position.y_center = y_center;
        }
    }

    const std::unordered_map<ItemType, int> item_prices =
    {
            {ItemType::CAPE, 15 * 1000},
            {ItemType::JETPACK, 20 * 1000},
            {ItemType::PISTOL, 10 * 1000},
            {ItemType::SHOTGUN, 20 * 1000},
            {ItemType::SPIKE_SHOES, 5 * 1000},
            {ItemType::SPRING_SHOES, 8 * 1000},
            {ItemType::MITT, 5 * 1000},
            {ItemType::GLOVE, 13 * 1000},
            {ItemType::COMPASS, 6 * 1000},
            {ItemType::BOMB_BAG, 5 * 1000},
            {ItemType::ROPE_PILE, 5 * 1000},
    };

    const std::unordered_map<ShopTheme, std::vector<ItemType>> items_in_theme =
    {
            {
                    ShopTheme::WEAPONS,
                    {ItemType::SHOTGUN, ItemType::PISTOL, ItemType::SPIKE_SHOES, ItemType::ROPE_PILE, ItemType::BOMB_BAG}
            },
            {
                    ShopTheme::CLOTHES,
                    {ItemType::CAPE, ItemType::SPRING_SHOES, ItemType::COMPASS, ItemType::GLOVE, ItemType::MITT, ItemType::JETPACK}
            },
    };
}

Shop::Shop(bool robbed) : _robbed(robbed)
{
    auto& tile_batch = Level::instance().get_tile_batch();
    for (int x_room = 0; x_room < Consts::ROOMS_COUNT_WIDTH; x_room++)
    {
        for (int y_room = 0; y_room < Consts::ROOMS_COUNT_HEIGHT; y_room++)
        {
            if (tile_batch.get_room_type_at(x_room, y_room) == RoomType::SHOP_LEFT ||
                tile_batch.get_room_type_at(x_room, y_room) == RoomType::SHOP_RIGHT)
            {
                _shop_center.x_center = ((0.5f + x_room) * Consts::ROOM_WIDTH_TILES);
                _shop_center.y_center = ((0.5f + y_room) * Consts::ROOM_HEIGHT_TILES);
                _shop_zone.width = Consts::ROOM_WIDTH_TILES - 3.0f;
                _shop_zone.height = Consts::ROOM_HEIGHT_TILES;
                break;
            }
        }
    }
        
    const auto theme = get_random_theme();

    _items = items_in_theme.at(theme);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(_items.begin(), _items.end(), g);
}

entt::entity Shop::make_next_item(float x_center, float y_center)
{
    if (_items_made >= _items.size())
    {
        return entt::null;
    }

    auto item_type = _items.at(_items_made);
    auto item_entity = make_item(item_type);
    _items_made++;

    assert(item_prices.find(item_type) != item_prices.end());
    set_position(item_entity, x_center, y_center);

    if (!_robbed)
    {
        attach_saleable_component(item_entity, item_prices.at(item_type));
    }

    return item_entity;
}

void Shop::attach_saleable_component(entt::entity item, int price)
{
    auto& registry = EntityRegistry::instance().get_registry();
    registry.emplace<SaleableComponent>(item, price, item, _shop_zone, _shop_center);
}
