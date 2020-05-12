#pragma once

#include "RenderEntity.hpp"
#include "GameObject.hpp"
#include "components/PhysicsComponent.hpp"
#include "components/QuadComponent.hpp"
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

    RenderEntityID _render_entity_id;
    bool _facing_left;
    float _animation_update_delta_ms = 0;

    // Position of the CENTER of the body
    float _x = 0;
    float _y = 0;

    friend class PhysicsComponent;
    PhysicsComponent _physics_component;

    friend class QuadComponent;
    QuadComponent _quad_component;

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
