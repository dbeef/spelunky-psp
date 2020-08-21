#pragma once

#include <memory>
#include <vector>
#include <cstdint>

class GameEntity;

class GameEntitySystem
{
public:
    GameEntitySystem();
    void update(std::uint32_t delta_t);
    void add(std::shared_ptr<GameEntity> entity);
    void remove(const std::shared_ptr<GameEntity>& entity);
    void remove_all();
private:
    std::vector<std::shared_ptr<GameEntity>> _entities;
};
