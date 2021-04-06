#pragma once

struct Point2D
{
    float x;
    float y;

    Point2D(float x, float y) : x(x), y(y) {}
    Point2D() = default;

    bool operator==(const Point2D& other) const
    {
        return this->x == other.x && this->y == other.y;
    }
};
