#pragma once

struct Vector2D
{
    float x;
    float y;

    Vector2D(float x, float y) : x(x), y(y) {}
    Vector2D() = default;

    bool operator==(const Vector2D& other) const
    {
        return this->x == other.x && this->y == other.y;
    }
};
