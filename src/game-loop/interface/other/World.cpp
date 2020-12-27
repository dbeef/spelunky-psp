#include "other/World.hpp"
#include "TileBatch.hpp"

World::World(GameEntitiesSPtr entities)
        : _entities(std::move(entities))
        , _tile_batch(std::make_shared<TileBatch>())
{
}
