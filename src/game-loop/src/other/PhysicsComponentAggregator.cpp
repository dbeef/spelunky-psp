#include "other/PhysicsComponentAggregator.hpp"
#include "components/PhysicsComponent.hpp"

#include <cassert>
#include <algorithm>

REGISTER_SINGLETON_INSTANCE(PhysicsComponentAggregator)

void PhysicsComponentAggregator::add(PhysicsComponent* component, PhysicsComponentType type)
{
    assert(component);
    _components.emplace_back(std::make_pair(component, type));
}

void PhysicsComponentAggregator::remove(PhysicsComponent* component)
{
    assert(component);
    auto iter = std::remove_if(_components.begin(), _components.end(),
                [component](const std::pair<PhysicsComponent*, PhysicsComponentType> other) { return component == other.first; });

    if (iter != _components.end())
    {
        _components.erase(iter);
    }
    else
    {
        assert(false);
    }
}

bool PhysicsComponentAggregator::is_collision(PhysicsComponent* component, PhysicsComponentType type)
{
    for (const auto& p : _components)
    {
        if (p.second == type && p.first->is_collision(*component))
        {
            return true;
        }
    }
    return false;
}
