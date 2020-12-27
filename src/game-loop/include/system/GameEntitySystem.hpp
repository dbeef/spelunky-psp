#pragma once

#include <memory>
#include <vector>
#include <cstdint>

class World;
class GameEntity;
using GameEntities = std::vector<std::shared_ptr<GameEntity>>;
using GameEntitiesSPtr = std::shared_ptr<GameEntities>;

class GameEntitySystem
{
public:
    GameEntitySystem();
    void update(World* world, std::uint32_t delta_t);
    void add(std::shared_ptr<GameEntity> entity);
    void add(std::vector<std::shared_ptr<GameEntity>>& entities);
    void remove(const std::shared_ptr<GameEntity>& entity);
    void remove_all();
    const GameEntitiesSPtr& get_entities() const { return _entities; }
private:
    GameEntitiesSPtr _entities;
};
