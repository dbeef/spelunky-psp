#pragma once

#include <vector>

#include "Vertex.hpp"
#include "IndexType.hpp"

struct Mesh
{
    std::vector <Vertex> vertices;
    std::vector <IndexType> indices;
};
