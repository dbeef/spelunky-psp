#include "other/PhysicsComponentAggregator.hpp"
#include "components/PhysicsComponent.hpp"

#include <cassert>
#include <algorithm>

REGISTER_SINGLETON_INSTANCE(PhysicsComponentAggregator)

void PhysicsComponentAggregator::add(PhysicsComponent* component)
{
    assert(component);
    _physics_components.push_back(component);
}

void PhysicsComponentAggregator::remove(PhysicsComponent* component)
{
    assert(component);
    auto iter = std::remove_if(_physics_components.begin(), _physics_components.end(),
                               [component](const PhysicsComponent* other) { return component == other; });

    if (iter != _physics_components.end())
    {
        _physics_components.erase(iter);
    }
    else
    {
        assert(false);
    }
}
