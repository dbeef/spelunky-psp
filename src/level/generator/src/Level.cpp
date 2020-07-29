#include <cassert>

#include "Level.hpp"

Level* Level::_instance = nullptr;

Level &Level::instance()
{
    assert(_instance);
    return *_instance;
}

void Level::init()
{
    assert(!_instance);
    _instance = new Level();
}

void Level::dispose()
{
    assert(_instance);
    delete _instance;
    _instance = nullptr;
}
