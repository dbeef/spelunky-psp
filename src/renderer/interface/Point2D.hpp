#pragma once

struct Point2D
{
    float x;
    float y;

    bool operator==(const Point2D& other) const
    {
        return this->x == other.x && this->y == other.y;
    }
};
