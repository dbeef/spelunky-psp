#pragma once

#include "patterns/Singleton.hpp"
#include "TileBatch.hpp"

class Level : public Singleton<Level>
{
public:
    DELETE_COPY_MOVE_CONSTRUCTORS(Level)
    FRIEND_SINGLETON(Level)

    TileBatch& get_tile_batch() { return *_tile_batch; };
    void recreate_tile_batch(int width_x, int height_y) { _tile_batch = std::make_unique<TileBatch>(width_x, height_y); }
private:
    Level() = default;
    std::unique_ptr<TileBatch> _tile_batch = std::make_unique<TileBatch>();
};
