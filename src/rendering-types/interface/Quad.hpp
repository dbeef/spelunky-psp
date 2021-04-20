#pragma once

#include "Mesh.hpp"

class Quad
{
public:

    Quad() = default;
    Quad(const Quad& other);

    Vertex* get_vertices_transformed() { return _transformed_vertices; };
    Vertex* get_vertices_base() { return _base_vertices; };
    IndexType* get_indices() { return _indices; };

    static std::size_t get_indices_count() { return 6; }
    static std::size_t get_vertices_count() { return 4; }

    void set_scale(float x, float y);
    void set_scale(float x, float y, float z);
    void set_translation(float x, float y);
    void set_translation(float x, float y, float z);
    void add_translation(float x, float y, float z);
    void write();

    float get_translation_x() const { return _translation.x; }
    float get_translation_y() const { return _translation.y; }
    float get_translation_z() const { return _translation.z; }

private:

    struct
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
    } _translation;

    struct
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
    } _scale;

    Vertex _transformed_vertices[4] = {0.0f};
    Vertex _base_vertices[4] = {0.0f};
    IndexType _indices[6] = {0};
};
