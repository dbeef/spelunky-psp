#pragma once

#include "prefabs/npc/CavemanStates.hpp"
#include "components/damage/HitpointComponent.hpp"
#include "components/damage/TakeProjectileDamageComponent.hpp"
#include "components/damage/TakeJumpOnTopDamage.hpp"
#include "components/damage/TakeMeleeDamageComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"

namespace prefabs
{
    class CavemanDeathObserver final : public Observer<DeathEvent>
    {
    public:
        explicit CavemanDeathObserver(entt::entity caveman) : _caveman(caveman) {}
        void on_notify(const DeathEvent*) override;
    private:
        const entt::entity _caveman;
    };

    class CavemanProjectileDamageObserver final : public Observer<TakenProjectileDamageEvent>
    {
    public:
        explicit CavemanProjectileDamageObserver(entt::entity caveman) : _caveman(caveman) {}
        void on_notify(const TakenProjectileDamageEvent *) override;
    private:
        const entt::entity _caveman;
    };

    class CavemanMeleeDamageObserver final : public Observer<TakenMeleeDamageEvent>
    {
    public:
        explicit CavemanMeleeDamageObserver(entt::entity caveman) : _caveman(caveman) {}
        void on_notify(const TakenMeleeDamageEvent *) override;
    private:
        const entt::entity _caveman;
    };

    class CavemanJumpOnTopDamageObserver final : public Observer<TakenJumpOnTopDamageEvent>
    {
    public:
        explicit CavemanJumpOnTopDamageObserver(entt::entity caveman) : _caveman(caveman) {}
        void on_notify(const TakenJumpOnTopDamageEvent *) override;
    private:
        const entt::entity _caveman;
    };

    class CavemanScript final : public ScriptBase
    {
    public:
        friend class CavemanDeathObserver;
        friend class CavemanProjectileDamageObserver;
        friend class CavemanMeleeDamageObserver;
        friend class CavemanJumpOnTopDamageObserver;

        friend class CavemanRunningState;
        friend class CavemanStandingState;
        friend class CavemanDeadState;
        friend class CavemanHeldDeadState;
        friend class CavemanHeldStunnedState;
        friend class CavemanFallingState;
        friend class CavemanBouncingState;
        friend class CavemanStunnedState;

        explicit CavemanScript(entt::entity caveman)
            : _death_observer(caveman)
            , _melee_observer(caveman)
            , _projectile_observer(caveman)
            , _jump_on_top_observer(caveman)
        {
        }

        void update(entt::entity owner, uint32_t delta_time_ms) override;

        CavemanDeathObserver* get_death_observer() { return &_death_observer; }
        CavemanProjectileDamageObserver* get_projectile_damage_observer() { return &_projectile_observer; }
        CavemanMeleeDamageObserver* get_melee_damage_observer() { return &_melee_observer; }
        CavemanJumpOnTopDamageObserver* get_jump_on_top_damage_observer() { return &_jump_on_top_observer; }

        void add_take_melee_damage_component(entt::entity);
        void add_take_jump_on_top_damage_component(entt::entity);
    private:

        bool _angry = false;
        int32_t _stunned_timer_ms = 0;

        CavemanDeathObserver _death_observer;
        CavemanProjectileDamageObserver _projectile_observer;
        CavemanMeleeDamageObserver _melee_observer;
        CavemanJumpOnTopDamageObserver _jump_on_top_observer;

        struct
        {
            CavemanStandingState standing;
            CavemanRunningState running;
            CavemanDeadState dead;
            CavemanHeldDeadState held_dead;
            CavemanFallingState falling;
            CavemanBouncingState bouncing;
            CavemanStunnedState stunned;
            CavemanHeldStunnedState held_stunned;
            CavemanBaseState* current = &standing;
        } _states;

        void enter_state(CavemanBaseState* new_state, entt::entity owner);
    };
}
