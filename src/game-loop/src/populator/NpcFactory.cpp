#include "populator/NpcFactory.hpp"
#include "EntityRegistry.hpp"
#include "components/generic/PositionComponent.hpp"
#include "prefabs/npc/Snake.hpp"
#include "prefabs/npc/Bat.hpp"
#include "prefabs/npc/Caveman.hpp"
#include "prefabs/npc/Spider.hpp"
#include "prefabs/npc/Skeleton.hpp"
#include "prefabs/npc/Shopkeeper.hpp"
#include "prefabs/npc/Damsel.hpp"
#include "prefabs/npc/BlueFrog.hpp"
#include "prefabs/npc/RedFrog.hpp"

entt::entity NpcFactory::make(NpcType npc_type) {
    switch (npc_type)
    {
        case NpcType::SNAKE: return prefabs::Snake::create();
        case NpcType::BAT: return prefabs::Bat::create();
        case NpcType::CAVEMAN: return prefabs::Caveman::create();
        case NpcType::SPIDER: return prefabs::Spider::create();
        case NpcType::SKELETON: return prefabs::Skeleton::create();
        case NpcType::SHOPKEEPER: return prefabs::Shopkeeper::create();
        case NpcType::BLUE_FROG: return prefabs::BlueFrog::create();
        case NpcType::RED_FROG: return prefabs::RedFrog::create();
    }
    assert(false);
    return {};
}

entt::entity NpcFactory::make(NpcType npc_type, float pos_x, float pos_y) {
    const auto out_entity = make(npc_type);
    auto& registry = EntityRegistry::instance().get_registry();
    auto& position = registry.get<PositionComponent>(out_entity);
    position.x_center = pos_x;
    position.y_center = pos_y;
    return out_entity;
}