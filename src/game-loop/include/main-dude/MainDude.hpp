#pragma once

#include "RenderEntity.hpp"
#include "GameObject.hpp"
#include "components/PhysicsComponent.hpp"
#include "MainDudeSpritesheetFrames.hpp"
#include "main-dude/states/MainDudeRunning.hpp"
#include "main-dude/states/MainDudeStanding.hpp"

#include <vector>

class MainDudeBaseState;
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

    bool _facing_left;
    MainDudeSpritesheetFrames _current_frame;
    float _animation_update_delta_ms = 0;

    // Position of the CENTER of the body
    float _x = 0;
    float _y = 0;

    friend class PhysicsComponent;
    PhysicsComponent _physics_component;

    friend class MainDudeBaseState;
    friend class MainDudeRunning;
    friend class MainDudeStanding;

    struct
    {
        MainDudeBaseState* current = nullptr;
        MainDudeRunning running;
        MainDudeStanding standing;
    } _states;
};
