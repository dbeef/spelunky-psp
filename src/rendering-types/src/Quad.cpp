#include "Quad.hpp"
#include <cstring>

void Quad::set_scale(float x, float y, float z)
{
    _scale.x = x;
    _scale.y = y;
    _scale.z = z;
}

void Quad::set_scale(float x, float y)
{
    _scale.x = x;
    _scale.y = y;
}

void Quad::add_translation(float x, float y, float z)
{
    _translation.x += x;
    _translation.y += y;
    _translation.z += z;
}

void Quad::set_translation(float x, float y, float z)
{
    _translation.x = x;
    _translation.y = y;
    _translation.z = z;
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
        out.z = ((in.z - 0.5f) * _scale.z) + _translation.z;

        out.u = in.u;
        out.v = in.v;
    }

}

Quad::Quad(const Quad &other)
{
    _translation.x = other._translation.x;
    _translation.y = other._translation.y;
    _translation.z = other._translation.z;

    _scale.x = other._scale.x;
    _scale.y = other._scale.y;
    _scale.z = other._scale.z;

    int index = 0;
    for (const auto& vertex : other._transformed_vertices)
    {
        _transformed_vertices[index] = vertex;
        index++;
    }

    index = 0;
    for (const auto& vertex : other._base_vertices)
    {
        _base_vertices[index] = vertex;
        index++;
    }

    index = 0;
    for (const auto& i : other._indices)
    {
        _indices[index] = i;
        index++;
    }
}
