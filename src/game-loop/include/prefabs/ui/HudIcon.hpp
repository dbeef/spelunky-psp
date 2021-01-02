#pragma once

#include "entt/entt.hpp"
#include "spritesheet-frames/HUDSpritesheetFrames.hpp"
#include <string>

namespace prefabs
{
    struct HudIcon
    {
        static entt::entity create(float x, float y, HUDSpritesheetFrames frame);
        static float getIconSizeWorldUnits();
    };
}
