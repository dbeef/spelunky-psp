#pragma once

#include "patterns/Singleton.hpp"
#include "TileBatch.hpp"

class Level : public Singleton<Level>
{
public:
    DELETE_COPY_MOVE_CONSTRUCTORS(Level)
    FRIEND_SINGLETON(Level)

    TileBatch& get_tile_batch() { return _tile_batch; };
private:
    Level() = default;
    TileBatch _tile_batch;
};
