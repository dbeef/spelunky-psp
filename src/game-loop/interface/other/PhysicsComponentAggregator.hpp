#pragma once

#include "patterns/Singleton.hpp"
#include "other/PhysicsComponentType.hpp"

#include <vector>

class PhysicsComponent;

class PhysicsComponentAggregator : public Singleton<PhysicsComponentAggregator>
{
public:
    DELETE_COPY_MOVE_CONSTRUCTORS(PhysicsComponentAggregator)
    FRIEND_SINGLETON(PhysicsComponentAggregator)

    void add(PhysicsComponent*);
    void remove(PhysicsComponent*);
    std::vector<PhysicsComponent*>& get_physics_components() { return _physics_components; }

private:
    std::vector<PhysicsComponent*> _physics_components;
    PhysicsComponentAggregator() = default;
};
