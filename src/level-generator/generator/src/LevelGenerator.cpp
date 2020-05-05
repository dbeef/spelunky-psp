#include <cassert>

#include "LevelGenerator.hpp"

LevelGenerator* LevelGenerator::_level_generator = nullptr;

LevelGenerator &LevelGenerator::instance()
{
    assert(_level_generator);
    return *_level_generator;
}

void LevelGenerator::init()
{
    assert(!_level_generator);
    _level_generator = new LevelGenerator();
}

void LevelGenerator::dispose()
{
    assert(_level_generator);
    delete _level_generator;
    _level_generator = nullptr;
}
