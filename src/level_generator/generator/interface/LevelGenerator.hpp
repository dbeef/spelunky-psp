#pragma once

class LevelGenerator
{
public:

    static LevelGenerator& instance();
    static void init();
    static void dispose();

private:

    static LevelGenerator* _level_generator;
};
