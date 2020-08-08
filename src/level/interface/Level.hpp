#pragma once

#include "patterns/Singleton.hpp"
#include "TileBatch.hpp"

class Level : public Singleton<Level>
{
public:
    TileBatch& get_tile_batch() { return _tile_batch; };
private:
    TileBatch _tile_batch;
};
