#pragma once

#include "RenderEntity.hpp"
#include "GameObject.hpp"

#include <vector>

class MainDude : public GameObject
{
public:
    MainDude();
    ~MainDude();
    void update() override {};
private:

    std::vector<Vertex> _mesh;
    std::vector<IndicesType> _indices;
    RenderEntity _render_entity;
};
