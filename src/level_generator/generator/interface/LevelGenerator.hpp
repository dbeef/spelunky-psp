#pragma once

#include "Level.hpp"

class LevelGenerator
{
public:

    static LevelGenerator& instance();
    static void init();
    static void dispose();

    inline Level& getLevel() { return _level; };

private:

    Level _level;

    static LevelGenerator* _level_generator;
};
