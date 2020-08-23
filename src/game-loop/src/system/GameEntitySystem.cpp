#include "system/GameEntitySystem.hpp"
#include "game-entities/GameEntity.hpp"

#include <algorithm>
#include <cassert>
#include <utility>

void GameEntitySystem::add(std::shared_ptr<GameEntity> entity)
{
    _entities.emplace_back(std::move(entity));
}

void GameEntitySystem::remove(const std::shared_ptr<GameEntity> &entity)
{
    const auto it = std::remove_if(_entities.begin(), _entities.end(), [entity](const auto& other){ return entity == other;});
    if (it != _entities.end())
    {
        _entities.erase(it);
    }
    else
    {
        assert(false);
    }
}

void GameEntitySystem::remove_all()
{
    _entities.clear();
}

GameEntitySystem::GameEntitySystem()
{
    // High initial capacity to avoid time-costly collection reallocation in mid-frame:
    const std::size_t STARTING_CAPACITY = 256;
    _entities.resize(STARTING_CAPACITY);
}

void GameEntitySystem::update(std::uint32_t delta_t)
{
    for (const auto& entity : _entities)
    {
        entity->update(delta_t);
    }

    const auto it = std::remove_if(_entities.begin(), _entities.end(), [](const auto& game_object)
    {
        return game_object->is_marked_for_disposal();
    });

    if (it != _entities.end())
    {
        _entities.erase(it, _entities.end());
    }
}

void GameEntitySystem::add(std::vector<std::shared_ptr<GameEntity>>& other)
{
    _entities.insert(_entities.end(), other.begin(), other.end());
}
