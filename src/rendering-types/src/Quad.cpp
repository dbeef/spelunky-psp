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

        // (0.5, 0.5) offset to make quad center to be at 0.0 origin:
        out.x = ((in.x - 0.5f) * _scale.x) + _translation.x;
        out.y = ((in.y - 0.5f) * _scale.y) + _translation.y;

        out.u = in.u;
        out.v = in.v;
    }
}
