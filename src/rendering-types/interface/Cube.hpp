#pragma once

#include "Mesh.hpp"
#include "Quad.hpp"

#include <vector>

class Cube
{
public:

    explicit Cube(const Quad&);
    Cube() = delete;

    std::vector<Vertex>& get_vertices_transformed() { return _vertices; };
    std::vector<IndexType>& get_indices() { return _indices; };

    void write();

private:

    Quad _base_quad;
    std::vector<IndexType> _indices;
    std::vector<Vertex> _vertices;
};
