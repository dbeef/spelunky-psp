#include "prefabs/npc/FakeSkeleton.hpp"
#include "prefabs/other/Bones.hpp"
#include "prefabs/items/Skull.hpp"

#include "EntityRegistry.hpp"

void prefabs::FakeSkeleton::create()
{
    create(0, 0);
}

void prefabs::FakeSkeleton::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    prefabs::Skull::create(pos_x_center, pos_y_center);
    prefabs::Bones::create(pos_x_center, pos_y_center);
}
