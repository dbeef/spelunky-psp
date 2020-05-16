#pragma once

#include "RenderEntity.hpp"
#include "GameObject.hpp"
#include "MainDudeSpritesheetFrames.hpp"

#include "components/PhysicsComponent.hpp"
#include "components/QuadComponent.hpp"
#include "components/InputComponent.hpp"
#include "components/AnimationComponent.hpp"

#include "main-dude/states/MainDudeRunning.hpp"
#include "main-dude/states/MainDudeStanding.hpp"
#include "main-dude/states/MainDudePushing.hpp"
#include "main-dude/states/MainDudeCrawlingState.hpp"
#include "main-dude/states/MainDudeDuckingState.hpp"

#include <vector>

class MainDudeBaseState;
class MainDude : public GameObject
{
public:

    MainDude();
    void update(uint32_t delta_time_ms) override;

private:

    friend class PhysicsComponent;
    friend class QuadComponent;
    friend class InputComponent;
    friend class AnimationComponent;
    PhysicsComponent _physics;
    QuadComponent _quad;
    InputComponent _input;
    AnimationComponent _animation;

    friend class MainDudeBaseState;
    friend class MainDudeRunning;
    friend class MainDudeStanding;
    friend class MainDudePushing;
    friend class MainDudeCrawling;
    friend class MainDudeDucking;
    struct
    {
        MainDudeBaseState* current = nullptr;
        MainDudeRunning running;
        MainDudeStanding standing;
        MainDudePushing pushing;
        MainDudeCrawling crawling;
        MainDudeDucking ducking;
    } _states;

    struct
    {
        bool facing_left;
    } _other;
};
