#pragma once

#include "patterns/Singleton.hpp"
#include "entt/entt.hpp"

#include <memory>

class EntityRegistry : public Singleton<EntityRegistry>
{
public:

    DELETE_COPY_MOVE_CONSTRUCTORS(EntityRegistry)
    FRIEND_SINGLETON(EntityRegistry)

    entt::registry& get_registry() { return _registry; }
    const entt::registry& get_registry() const { return _registry; }

private:

    EntityRegistry() = default;

    entt::registry _registry;
};
