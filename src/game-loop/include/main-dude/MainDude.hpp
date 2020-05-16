#pragma once

#include "RenderEntity.hpp"
#include "GameObject.hpp"
#include "MainDudeSpritesheetFrames.hpp"

#include "components/PhysicsComponent.hpp"
#include "components/QuadComponent.hpp"
#include "components/AnimationComponent.hpp"

#include "main-dude/states/MainDudeRunningState.hpp"
#include "main-dude/states/MainDudeExitingState.hpp"
#include "main-dude/states/MainDudeStandingState.hpp"
#include "main-dude/states/MainDudePushingState.hpp"
#include "main-dude/states/MainDudeCrawlingState.hpp"
#include "main-dude/states/MainDudeDuckingState.hpp"
#include "main-dude/states/MainDudeFallingState.hpp"
#include "main-dude/states/MainDudeJumpingState.hpp"
#include "main-dude/states/MainDudeThrowingState.hpp"

#include <vector>

class MainDudeBaseState;
class Input;

class MainDude : public GameObject
{
public:

    MainDude();
    void update(uint32_t delta_time_ms) override;

private:

    void handle_input(const Input& input);
    MapTile* is_overlaping_exit() const;

    friend class PhysicsComponent;
    friend class QuadComponent;
    friend class AnimationComponent;
    PhysicsComponent _physics;
    QuadComponent _quad;
    AnimationComponent _animation;

    friend class MainDudeBaseState;
    friend class MainDudeRunningState;
    friend class MainDudeStandingState;
    friend class MainDudePushingState;
    friend class MainDudeCrawling;
    friend class MainDudeDucking;
    friend class MainDudeFalling;
    friend class MainDudeJumping;
    friend class MainDudeThrowingState;
    friend class MainDudeExitingState;
    struct
    {
        MainDudeBaseState* current = nullptr;
        MainDudeRunningState running;
        MainDudeStandingState standing;
        MainDudePushingState pushing;
        MainDudeCrawling crawling; // TODO: Postfixes
        MainDudeDucking ducking; // TODO: Postfixes
        MainDudeFalling falling; // TODO: Postfixes
        MainDudeJumping jumping; // TODO: Postfixes
        MainDudeThrowingState throwing;
        MainDudeExitingState exiting;
    } _states;

    struct
    {
        bool facing_left;
    } _other;

    static constexpr float DEFAULT_DELTA_X = 0.01f;
    static constexpr float DEFAULT_MAX_X_VELOCITY = 0.050f;

    static constexpr float MAX_RUNNING_VELOCITY_X = 0.125f;
    static constexpr float MAX_CRAWLING_VELOCITY_X = 0.008f;
    static constexpr float CRAWLING_DELTA_X = 0.008f;

    static constexpr float JUMP_SPEED = 0.35f;
};
