#pragma once

#include <utility>
#include <vector>
#include <memory>
#include <utility>

#include "system/GameEntitySystem.hpp"

class MainDude;
class TileBatch;

class World
{
public:

    explicit World(GameEntitiesSPtr entities);

    std::shared_ptr<TileBatch>& get_tile_batch() { return _tile_batch; }
    std::shared_ptr<MainDude>& get_main_dude() { return _main_dude; }
    const GameEntitiesSPtr& get_game_entities() { return _entities; }

private:
    std::shared_ptr<TileBatch> _tile_batch;
    std::shared_ptr<MainDude> _main_dude;
    const GameEntitiesSPtr _entities;
};
