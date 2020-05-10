#pragma once

#include "RenderEntity.hpp"
#include "GameObject.hpp"
#include "components/PhysicsComponent.hpp"
#include "components/InputComponent.hpp"
#include "MainDudeState.hpp"
#include "MainDudeSpritesheetFrames.hpp"

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

    MainDudeState _state;
    MainDudeSpritesheetFrames _current_frame;
    float _animation_update_delta_ms = 0;

    // Position of the CENTER of the body
    float _x = 0;
    float _y = 0;

    friend class InputComponent;
    InputComponent _input_component;
    friend class PhysicsComponent;
    PhysicsComponent _physics_component;
};
