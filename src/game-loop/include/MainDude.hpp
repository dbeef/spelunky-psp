#pragma once

#include "RenderEntity.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "InputComponent.hpp"

#include <vector>

class MainDude : public GameObject
{
public:

    MainDude();
    ~MainDude() override;
    void update() override;

private:

    std::vector<Vertex> _mesh;
    std::vector<IndicesType> _indices;
    RenderEntity _render_entity;

    float _x = 0;
    float _y = 0;

    friend class InputComponent;
    InputComponent _input_component;
    friend class PhysicsComponent;
    PhysicsComponent _physics_component;
};
