#pragma once

#include "RenderEntity.hpp"

#include "spritesheet-frames/MainDudeSpritesheetFrames.hpp"
#include "patterns/Subject.hpp"

#include "MapTile.hpp"
#include "MapTileType.hpp"

#include "main-dude/states/MainDudeRunningState.hpp"
#include "main-dude/states/MainDudeClimbingState.hpp"
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
#include "main-dude/MainDudeObservers.hpp"

#include <vector>
#include <memory>
#include <components/generic/ItemCarrierComponent.hpp>

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
    void enter_climbing_state();
    void enter_stunned_state();

    bool entered_door() const { return _other.entered_door; }

    MainDudeClimbingObserver* get_climbing_observer()
    {
        return _climbing_observer.get();
    }

    MainDudeFallObserver* get_fall_observer()
    {
        return _fall_observer.get();
    }

    MainDudeDeathObserver* get_death_observer()
    {
        return _death_observer.get();
    }

    MainDudeNpcDamageObserver* get_npc_damage_observer()
    {
        return _npc_damage_observer.get();
    }

    MainDudeExplosionDamageObserver* get_explosion_damage_observer()
    {
        return _explosion_observer.get();
    }

    MainDudeProjectileDamageObserver* get_projectile_damage_observer()
    {
        return _projectile_observer.get();
    }

    MainDudeSpikesDamageObserver* get_spikes_damage_observer()
    {
        return _spikes_observer.get();
    }

private:

    void enter_if_different(MainDudeBaseState*);
    MapTile* is_overlaping_tile(MapTileType, PhysicsComponent& physics, PositionComponent& position) const;

    // Returns true on successful request.
    bool hang_off_cliff_left_gloves(ItemCarrierComponent& items, PhysicsComponent& physics, PositionComponent& position);
    bool hang_off_cliff_right_gloves(ItemCarrierComponent& items, PhysicsComponent& physics, PositionComponent& position);

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
    friend class MainDudeClimbingState;
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
        MainDudeClimbingState climbing;
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

    std::shared_ptr<MainDudeClimbingObserver> _climbing_observer;
    std::shared_ptr<MainDudeFallObserver> _fall_observer;
    std::shared_ptr<MainDudeDeathObserver> _death_observer;
    std::shared_ptr<MainDudeNpcDamageObserver> _npc_damage_observer;
    std::shared_ptr<MainDudeExplosionDamageObserver> _explosion_observer;
    std::shared_ptr<MainDudeProjectileDamageObserver> _projectile_observer;
    std::shared_ptr<MainDudeSpikesDamageObserver> _spikes_observer;

    static constexpr float DEFAULT_DELTA_X = 0.01f;
    static constexpr float DEFAULT_MAX_X_VELOCITY = 0.050f;

    static constexpr float MAX_RUNNING_VELOCITY_X = 0.15f;
    static constexpr float MAX_CRAWLING_VELOCITY_X = 0.008f;
    static constexpr float CRAWLING_DELTA_X = 0.008f;

    static constexpr float JUMP_SPEED = 0.165f;
};
