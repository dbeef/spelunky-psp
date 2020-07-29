#pragma once

#include "TileBatch.hpp"

class Level
{
public:

    static Level& instance();
    static void init();
    static void dispose();

    TileBatch& get_tile_batch() { return _tile_batch; };

private:

    TileBatch _tile_batch;
    static Level* _instance;
};
