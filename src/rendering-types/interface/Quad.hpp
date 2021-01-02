#pragma once

#include "Mesh.hpp"

class Quad
{
public:

    Vertex* get_vertices_transformed() { return _transformed_vertices; };
    Vertex* get_vertices_base() { return _base_vertices; };
    IndexType* get_indices() { return _indices; };

    static std::size_t get_indices_count() { return 6; }
    static std::size_t get_vertices_count() { return 4; }

    void set_scale(float x, float y);
    void set_translation(float x, float y);
    void write();

private:

    struct
    {
        float x = 0.0f;
        float y = 0.0f;
    } _translation;

    struct
    {
        float x = 0.0f;
        float y = 0.0f;
    } _scale;

    Vertex _transformed_vertices[4] = {0.0f};
    Vertex _base_vertices[4] = {0.0f};
    IndexType _indices[6] = {0};
};
