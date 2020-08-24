#pragma once

#include "RenderEntity.hpp"
#include "game-entities/GameEntity.hpp"
#include "spritesheet-frames/MainDudeSpritesheetFrames.hpp"
#include "patterns/Subject.hpp"

#include "components/PhysicsComponent.hpp"
#include "components/QuadComponent.hpp"
#include "components/AnimationComponent.hpp"

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

class MainDudeBaseState;
class Input;
class MapTile;

class MainDude : public GameEntity, public Subject<MainDudeEvent>
{
public:

    MainDude(float x_pos_center, float y_pos_center);

    void update(uint32_t delta_time_ms) override;

    float get_x_pos_center() const { return _physics.get_x_position(); }
    float get_y_pos_center() const { return _physics.get_y_position(); }

    uint8_t get_hearts() const { return _stats.hearts; }
    uint8_t get_dollars() const { return _stats.dollars; }
    uint8_t get_ropes() const { return _stats.ropes; }
    uint8_t get_bombs() const { return _stats.bombs; }

    void set_position_on_tile(MapTile* map_tile);
    void set_position(float x_center, float y_center) { _physics.set_position(x_center, y_center); }
    void set_velocity(float x, float y) { _physics.set_velocity(x, y); }

    void enter_level_summary_state();
    void enter_dead_state();
    void enter_standing_state();

    bool entered_door() const { return _other.entered_door; }

private:

    void set_starting_stats();
    void decrease_hearts(uint8_t amount);
    void handle_input(const Input& input);
    MapTile* is_overlaping_tile(MapTileType) const;

    // Returns true on successful request.
    bool hang_off_cliff_left();
    bool hang_off_cliff_right();

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
        int16_t hearts;
        int16_t ropes;
        int16_t bombs;
        int16_t dollars;
    } _stats;

    struct
    {
        bool facing_left;
        bool dead = false;
        bool entered_door = false;
    } _other;

    static constexpr float DEFAULT_DELTA_X = 0.01f;
    static constexpr float DEFAULT_MAX_X_VELOCITY = 0.050f;
    static constexpr float DEFAULT_MAX_Y_VELOCITY = 0.39f;

    static constexpr float MAX_RUNNING_VELOCITY_X = 0.15f;
    static constexpr float MAX_CRAWLING_VELOCITY_X = 0.008f;
    static constexpr float CRAWLING_DELTA_X = 0.008f;

    static constexpr float JUMP_SPEED = 0.165f;
};
