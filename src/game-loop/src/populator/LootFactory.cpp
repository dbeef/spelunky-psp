#include "populator/LootFactory.hpp"
#include "EntityRegistry.hpp"
#include "components/generic/PositionComponent.hpp"

entt::entity LootFactory::make(LootType loot_type) {
    switch (loot_type)
    {
        case LootType::NOTHING:break;
        case LootType::ANY:break;
        case LootType::SHOP_ITEM:break;
        case LootType::GOLDEN_IDOL:break;
    }
    assert(false);
    return {};
}

entt::entity LootFactory::make(LootType loot_type, float pos_x, float pos_y) {
    const auto out_entity = make(loot_type);
    auto& registry = EntityRegistry::instance().get_registry();
    auto& position = registry.get<PositionComponent>(out_entity);
    position.x_center = pos_x;
    position.y_center = pos_y;
    return out_entity;
}