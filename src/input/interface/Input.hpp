#pragma once

#include <cstdint>

class Input
{
public:

    static Input& instance();
    static void init();
    static void dispose();

private:

    static Input* _Input;
};
