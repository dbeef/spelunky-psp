#pragma once

#include "RenderEntity.hpp"

#include "spritesheet-frames/MainDudeSpritesheetFrames.hpp"
#include "patterns/Subject.hpp"

#include "MapTile.hpp"
#include "MapTileType.hpp"

#include "main-dude/states/MainDudeRunningState.hpp"
#include "main-dude/states/MainDudeClimbingLadderState.hpp"
#include "main-dude/states/MainDudeExitingState.hpp"
#include "main-dude/states/MainDudeStandingState.hpp"
#include "main-dude/states/MainDudePushingState.hpp"
#include "main-dude/states/MainDudeCrawlingState.hpp"
#include "main-dude/states/MainDudeDuckingState.hpp"
#include "main-dude/states/MainDudeFallingState.hpp"
#include "main-dude/states/MainDudeJumpingState.hpp"
#include "main-dude/states/MainDudeThrowingState.hpp"
#include "main-dude/states/MainDudeLevelSummaryState.hpp"
#include "main-dude/states/MainDudeCliffHangingState.hpp"
#include "main-dude/states/MainDudeLookingUpState.hpp"
#include "main-dude/states/MainDudeRunningLookingUpState.hpp"
#include "main-dude/states/MainDudeDeadState.hpp"
#include "main-dude/states/MainDudeStunnedState.hpp"
#include "main-dude/MainDudeEvent.hpp"

#include <vector>
#include <components/generic/PhysicsComponent.hpp>
#include <components/generic/PositionComponent.hpp>
#include <components/generic/QuadComponent.hpp>
#include <components/generic/AnimationComponent.hpp>

class MainDudeBaseState;
class Input;
class MapTile;

class MainDudeComponent : public Subject<MainDudeEvent>
{
public:

    explicit MainDudeComponent(entt::entity owner);
    MainDudeComponent(const MainDudeComponent&);

    void update(uint32_t delta_time_ms);

    void enter_level_summary_state();
    void enter_dead_state();
    void enter_standing_state();
    void enter_throwing_state();

    bool entered_door() const { return _other.entered_door; }

private:

    void enter_if_different(MainDudeBaseState*);
    MapTile* is_overlaping_tile(MapTileType, PhysicsComponent& physics, PositionComponent& position) const;

    // Returns true on successful request.
    bool hang_off_cliff_left(PhysicsComponent& physics, PositionComponent& position);
    bool hang_off_cliff_right(PhysicsComponent& physics, PositionComponent& position);

    friend class MainDudeBaseState;
    friend class MainDudeRunningState;
    friend class MainDudeStandingState;
    friend class MainDudePushingState;
    friend class MainDudeCrawlingState;
    friend class MainDudeDuckingState;
    friend class MainDudeFallingState;
    friend class MainDudeJumpingState;
    friend class MainDudeThrowingState;
    friend class MainDudeExitingState;
    friend class MainDudeLevelSummaryState;
    friend class MainDudeClimbingLadderState;
    friend class MainDudeCliffHangingState;
    friend class MainDudeLookingUpState;
    friend class MainDudeRunningLookingUpState;
    friend class MainDudeDeadState;
    friend class MainDudeStunnedState;
    struct
    {
        MainDudeBaseState* current = nullptr;
        MainDudeRunningState running;
        MainDudeStandingState standing;
        MainDudePushingState pushing;
        MainDudeCrawlingState crawling;
        MainDudeDuckingState ducking;
        MainDudeFallingState falling;
        MainDudeJumpingState jumping;
        MainDudeThrowingState throwing;
        MainDudeExitingState exiting;
        MainDudeClimbingLadderState climbing;
        MainDudeCliffHangingState cliff_hanging;
        MainDudeLookingUpState looking_up;
        MainDudeRunningLookingUpState running_looking_up;
        MainDudeLevelSummaryState level_summary;
        MainDudeDeadState dead;
        MainDudeStunnedState stunned;
    } _states;

    struct
    {
        bool dead = false;
        bool entered_door = false;
    } _other;

    entt::entity _owner = entt::null;

    static constexpr float DEFAULT_DELTA_X = 0.01f;
    static constexpr float DEFAULT_MAX_X_VELOCITY = 0.050f;
    static constexpr float DEFAULT_MAX_Y_VELOCITY = 0.39f;

    static constexpr float MAX_RUNNING_VELOCITY_X = 0.15f;
    static constexpr float MAX_CRAWLING_VELOCITY_X = 0.008f;
    static constexpr float CRAWLING_DELTA_X = 0.008f;

    static constexpr float JUMP_SPEED = 0.165f;
};
