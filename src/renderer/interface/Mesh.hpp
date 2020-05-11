#pragma once

#include <vector>

using IndicesType = int16_t;

struct Vertex
{
    float x;
    float y;
    float u;
    float v;
};

struct Mesh
{
    std::vector <Vertex> vertices;
    std::vector <IndicesType> indices;
};
