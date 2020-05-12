#include "Quad.hpp"
#include <cstring>

void Quad::scale(float x, float y)
{
    for (auto& vertex : _vertices)
    {
        vertex.x *= x;
        vertex.y *= y;
    }
}

void Quad::translate(float x, float y)
{
    for (auto& vertex : _vertices)
    {
        vertex.x += x;
        vertex.y += y;
    }
}
