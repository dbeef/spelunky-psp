#include "populator/ItemFactory.hpp"
#include "EntityRegistry.hpp"
#include "prefabs/items/Arrow.hpp"
#include "components/generic/PositionComponent.hpp"
#include "prefabs/items/Bomb.hpp"
#include "prefabs/items/Cape.hpp"
#include "prefabs/items/Chest.hpp"
#include "prefabs/items/Crate.hpp"
#include "prefabs/items/Jar.hpp"
#include "prefabs/items/Jetpack.hpp"
#include "prefabs/items/Pistol.hpp"
#include "prefabs/items/Rock.hpp"
#include "prefabs/items/Rope.hpp"
#include "prefabs/items/Shotgun.hpp"
#include "prefabs/items/Skull.hpp"
#include "prefabs/items/Whip.hpp"
#include "prefabs/items/SpikeShoes.hpp"
#include "prefabs/items/SpringShoes.hpp"
#include "prefabs/items/Mitt.hpp"
#include "prefabs/items/Glove.hpp"
#include "prefabs/items/Compass.hpp"
#include "prefabs/items/BombBag.hpp"
#include "prefabs/items/RopePile.hpp"
#include "prefabs/items/GoldenIdol.hpp"
#include "prefabs/items/Flare.hpp"

entt::entity ItemFactory::make(ItemType item_type) {
    switch (item_type)
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
        case ItemType::SPIKE_SHOES: return prefabs::SpikeShoes::create();
        case ItemType::SPRING_SHOES: return prefabs::SpringShoes::create();
        case ItemType::MITT: return prefabs::Mitt::create();
        case ItemType::GLOVE: return prefabs::Glove::create();
        case ItemType::COMPASS: return prefabs::Compass::create();
        case ItemType::BOMB_BAG: return prefabs::BombBag::create();
        case ItemType::ROPE_PILE: return prefabs::RopePile::create();
        case ItemType::GOLDEN_IDOL: return prefabs::GoldenIdol::create();
        case ItemType::FLARE: return prefabs::Flare::create();
    }
    assert(false);
    return {};
}

entt::entity ItemFactory::make(ItemType item_type, float pos_x, float pos_y) {
    const auto out_entity = make(item_type);
    auto& registry = EntityRegistry::instance().get_registry();
    auto& position = registry.get<PositionComponent>(out_entity);
    position.x_center = pos_x;
    position.y_center = pos_y;
    return out_entity;
}