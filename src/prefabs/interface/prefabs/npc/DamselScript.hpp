#pragma once

#include "entt/entt.hpp"
#include "prefabs/npc/DamselStates.hpp"
#include "EntityRegistry.hpp"
#include "components/damage/HitpointComponent.hpp"
#include "components/damage/TakeProjectileDamageComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"

namespace prefabs
{
    class DamselDeathObserver final : public Observer<DeathEvent>
    {
    public:
        explicit DamselDeathObserver(entt::entity damsel) : _damsel(damsel) {}
        void on_notify(const DeathEvent*) override;
    private:
        const entt::entity _damsel;
    };

    class DamselProjectileDamageObserver final : public Observer<ProjectileDamage_t>
    {
    public:
        explicit DamselProjectileDamageObserver(entt::entity damsel) : _damsel(damsel) {}
        void on_notify(const ProjectileDamage_t *) override;
    private:
        const entt::entity _damsel;
    };

    class DamselScript final : public ScriptBase
    {
    public:
        friend class DamselDeathObserver;
        friend class DamselProjectileDamageObserver;

        friend class DamselStandingState;
        friend class DamselRunningState;
        friend class DamselHeldState;
        friend class DamselExitingState;
        friend class DamselStunnedState;
        friend class DamselYellingState;
        friend class DamselDeadState;
        friend class DamselSmoochingState;
        friend class DamselFallingState;
        friend class DamselBouncingState;

        explicit DamselScript(entt::entity damsel, bool& damsel_rescued)
            : _death_observer(damsel)
            , _projectile_damage_observer(damsel)
            , _damsel_rescued(damsel_rescued)
        {}

        DamselDeathObserver* get_death_observer()
        {
            return &_death_observer;
        }

        DamselProjectileDamageObserver* get_projectile_damage_observer()
        {
            return &_projectile_damage_observer;
        }

        void update(entt::entity owner, uint32_t delta_time_ms) override;
        void set_panic(bool v) { _panic = v; }
        bool get_panic() const { return _panic; }
        void enter_smooching_state(entt::entity id)
        {
            if (_states.current != &_states.smooching)
            {
                enter_state(&_states.smooching, id);
            }
        }

    private:

        void enter_state(DamselBaseState* new_state, entt::entity owner);

        bool& _damsel_rescued;
        bool _panic = false;

        DamselDeathObserver _death_observer;
        DamselProjectileDamageObserver _projectile_damage_observer;

        struct
        {
            DamselStandingState standing;
            DamselRunningState running;
            DamselHeldState held;
            DamselExitingState exiting;
            DamselStunnedState stunned;
            DamselYellingState yelling;
            DamselDeadState dead;
            DamselSmoochingState smooching;
            DamselFallingState falling;
            DamselBouncingState bouncing;
            DamselBaseState* current = &standing;
        } _states;
    };
}
