#pragma once

#include "Mesh.hpp"

class Quad
{
public:

    Vertex* get_vertices() { return _vertices; };
    IndexType* get_indices() { return _indices; };

    static std::size_t get_indices_count() { return 6; }
    static std::size_t get_vertices_count() { return 4; }

    void scale(float x, float y);
    void translate(float x, float y);

private:

    Vertex _vertices[4];
    IndexType _indices[6];
};
