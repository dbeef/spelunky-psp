#pragma once

#include <cstdint>

#include "TextureType.hpp"
#include "Quad.hpp"
#include "MapTile.hpp"

class MainDude;

class InputComponent
{
public:

    void update(MainDude&, uint32_t delta_time_ms);

private:
    uint32_t _polling_timer = 0;
};
