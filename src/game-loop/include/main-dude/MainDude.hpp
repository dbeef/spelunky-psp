#pragma once

#include "RenderEntity.hpp"
#include "game-objects/GameObject.hpp"
#include "spritesheet-frames/MainDudeSpritesheetFrames.hpp"

#include "components/PhysicsComponent.hpp"
#include "components/QuadComponent.hpp"
#include "components/AnimationComponent.hpp"

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

#include <vector>

class MainDudeBaseState;
class Input;
class MapTile;

class MainDude : public GameObject
{
public:

    MainDude(float x_pos_center, float y_pos_center);
    void update(uint32_t delta_time_ms) override;

    float get_x_pos_center() const { return _physics.get_x_position(); }
    float get_y_pos_center() const { return _physics.get_y_position(); }

    void set_position_on_tile(MapTile* map_tile);
    void enter_level_summary_state();

    bool entered_door() const { return _other.entered_door; }

private:

    void handle_input(const Input& input);
    MapTile* is_overlaping_tile(MapTileType) const;

    friend class PhysicsComponent;
    friend class AnimationComponent;
    PhysicsComponent _physics;
    QuadComponent _quad;
    AnimationComponent _animation;

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
        MainDudeLevelSummaryState level_summary;
    } _states;

    struct
    {
        bool facing_left;
        bool entered_door = false;
    } _other;

    static constexpr float DEFAULT_DELTA_X = 0.01f;
    static constexpr float DEFAULT_MAX_X_VELOCITY = 0.050f;
    static constexpr float DEFAULT_MAX_Y_VELOCITY = 0.26f;

    static constexpr float MAX_RUNNING_VELOCITY_X = 0.15f;
    static constexpr float MAX_CRAWLING_VELOCITY_X = 0.008f;
    static constexpr float CRAWLING_DELTA_X = 0.008f;

    static constexpr float JUMP_SPEED = 0.25f;
};
