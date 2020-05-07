#pragma once

#include "RenderEntity.hpp"
#include "GameObject.hpp"
#include "components/PhysicsComponent.hpp"
#include "components/InputComponent.hpp"

#include <vector>

class MainDude : public GameObject
{
public:

    MainDude();
    ~MainDude() override;
    void update(uint32_t delta_time_ms) override;

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
