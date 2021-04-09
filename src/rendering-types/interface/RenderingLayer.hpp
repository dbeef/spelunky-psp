#pragma once

#include <cstdint>

enum class RenderingLayer : std::uint16_t
{
    LAYER_6_TILES,
    LAYER_5_PROPS,
    LAYER_4_DUDE,
    LAYER_3_ITEMS,
    LAYER_2_HUD,
    LAYER_1_OVERLAY,
    LAYER_0_OVERLAY_TEXT,
};
