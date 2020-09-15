#pragma once

#include "patterns/Singleton.hpp"
#include "other/PhysicsComponentType.hpp"

#include <vector>
#include <utility>

class PhysicsComponent;

class PhysicsComponentAggregator : public Singleton<PhysicsComponentAggregator>
{
public:
    DELETE_COPY_MOVE_CONSTRUCTORS(PhysicsComponentAggregator)
    FRIEND_SINGLETON(PhysicsComponentAggregator)

    void add(PhysicsComponent*, PhysicsComponentType);
    void remove(PhysicsComponent*);

    bool is_collision(PhysicsComponent* component, PhysicsComponentType type);

private:
    std::vector<std::pair<PhysicsComponent*, PhysicsComponentType>> _components;
    PhysicsComponentAggregator() = default;
};
