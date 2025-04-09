#pragma once

#include <random>
#include <functional>

#include <entt/entt.hpp>
#include "MapTile.hpp"
#include "TileBatch.hpp"

class Spawner {
public:
    static const int UNLIMITED = std::numeric_limits<int>::max();
    explicit Spawner(int chance_of_spawning_percent, int max_number_of_spawned,
                     const std::function<entt::entity(const MapTile*)>& spawn,
                     const std::function<bool(const TileBatch& tile_batch,
                     const MapTile* current_tile)> &spawn_predicate = {})
        : _chance_of_spawning_percent(chance_of_spawning_percent)
          , _max_number_of_spawned(max_number_of_spawned)
          , _spawn(spawn)
          , _spawn_predicate(spawn_predicate) {
    }

    Spawner(Spawner&& other) noexcept :
        _chance_of_spawning_percent(other._chance_of_spawning_percent),
        _max_number_of_spawned(other._max_number_of_spawned),
        _spawn(other._spawn),
        _spawn_predicate(other._spawn_predicate)
    {
    }

    Spawner(const Spawner& other) noexcept :
        _chance_of_spawning_percent(other._chance_of_spawning_percent),
        _max_number_of_spawned(other._max_number_of_spawned),
        _spawn(other._spawn),
        _spawn_predicate(other._spawn_predicate)
    {
    }

    Spawner& operator=(Spawner&& other) noexcept {
        if (this != &other) {
            _chance_of_spawning_percent = other._chance_of_spawning_percent;
            _max_number_of_spawned = other._max_number_of_spawned;
            _spawn = std::move(other._spawn);
            _spawn_predicate = std::move(other._spawn_predicate);
            _number_spawned = other._number_spawned;
        }
        return *this;
    }

    bool try_spawn(std::default_random_engine &engine, const TileBatch& tile_batch, const MapTile* map_tile) {
        if (_number_spawned >= _max_number_of_spawned) {
            return false;
        }

        if (!_spawn_predicate(tile_batch, map_tile)) {
            return false;
        }

        std::uniform_int_distribution<int> uniform_dist(0, 100);
        bool spawned = uniform_dist(engine) < _chance_of_spawning_percent;

        if (spawned) {
            _number_spawned++;
            _spawn(map_tile);
        }

        return spawned;
    }

private:
    int _number_spawned{};
    int _chance_of_spawning_percent;
    int _max_number_of_spawned;
    std::function<entt::entity(const MapTile*)> _spawn{};
    std::function<bool(const TileBatch& tile_batch, const MapTile* current_tile)> _spawn_predicate;
};
