#pragma once

struct ZoneComponent
{
    ZoneComponent(float width, float height) : width(width), height(height) {}
    ZoneComponent() = default;

    float width;
    float height;
};
