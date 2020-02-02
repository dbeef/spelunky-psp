#pragma once

class LevelRenderer
{
public:

    static LevelRenderer& instance();
    static void init();
    static void dispose();

private:

    static LevelRenderer* _level_renderer;
};
