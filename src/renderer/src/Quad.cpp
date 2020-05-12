#include "Quad.hpp"
#include <cstring>

void Quad::set_scale(float x, float y)
{
    _scale.x = x;
    _scale.y = y;
}

void Quad::set_translation(float x, float y)
{
    _translation.x = x;
    _translation.y = y;
}

void Quad::write()
{
    for (int index = 0; index < Quad::get_vertices_count(); ++index)
    {
        const Vertex& in = _base_vertices[index];
        Vertex& out = _transformed_vertices[index];

        out.x = (in.x * _scale.x) + _translation.x;
        out.y = (in.y * _scale.y) + _translation.y;

        out.u = in.u;
        out.v = in.v;
    }
}
