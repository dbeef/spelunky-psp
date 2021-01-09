#pragma once

#include <cstdint>

enum class RenderingLayer : std::uint16_t
{
    LAYER_5_TILES,
    LAYER_4_PROPS,
    LAYER_3_DUDE,
    LAYER_2_ITEMS,
    LAYER_1_UI_TEXT,
};
