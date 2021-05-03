#pragma once

#include "entt/entt.hpp"
#include "prefabs/npc/ShopkeeperStates.hpp"
#include "EntityRegistry.hpp"
#include "components/damage/HitpointComponent.hpp"
#include "components/damage/TakeProjectileDamageComponent.hpp"
#include "components/damage/TakeMeleeDamageComponent.hpp"
#include "components/damage/TakeJumpOnTopDamage.hpp"
#include "components/generic/ScriptingComponent.hpp"

namespace prefabs
{
    class ShopkeeperDeathObserver final : public Observer<DeathEvent>
    {
    public:
        explicit ShopkeeperDeathObserver(entt::entity shopkeeper) : _shopkeeper(shopkeeper) {}
        void on_notify(const DeathEvent*) override;
    private:
        const entt::entity _shopkeeper;
    };

    class ShopkeeperProjectileDamageObserver final : public Observer<ProjectileDamage_t>
    {
    public:
        explicit ShopkeeperProjectileDamageObserver(entt::entity shopkeeper) : _shopkeeper(shopkeeper) {}
        void on_notify(const ProjectileDamage_t *) override;
    private:
        const entt::entity _shopkeeper;
    };

    class ShopkeeperMeleeDamageObserver final : public Observer<MeleeDamage_t>
    {
    public:
        explicit ShopkeeperMeleeDamageObserver(entt::entity shopkeeper) : _shopkeeper(shopkeeper) {}
        void on_notify(const MeleeDamage_t *) override;
    private:
        const entt::entity _shopkeeper;
    };

    class ShopkeeperJumpOnTopDamageObserver final : public Observer<JumpOnTopDamage_t>
    {
    public:
        explicit ShopkeeperJumpOnTopDamageObserver(entt::entity shopkeeper) : _shopkeeper(shopkeeper) {}
        void on_notify(const JumpOnTopDamage_t *) override;
    private:
        const entt::entity _shopkeeper;
    };

    class ShopkeeperScript final : public ScriptBase
    {
    public:
        friend class ShopkeeperDeathObserver;
        friend class ShopkeeperProjectileDamageObserver;
        friend class ShopkeeperJumpOnTopDamageObserver;
        friend class ShopkeeperMeleeDamageObserver;

        friend class ShopkeeperStandingState;
        friend class ShopkeeperRunningState;
        friend class ShopkeeperStunnedState;
        friend class ShopkeeperDeadState;
        friend class ShopkeeperHeldDeadState;
        friend class ShopkeeperFallingState;
        friend class ShopkeeperBouncingState;

        explicit ShopkeeperScript(entt::entity shopkeeper)
            : _death_observer(shopkeeper)
            , _projectile_damage_observer(shopkeeper)
            , _melee_damage_observer(shopkeeper)
            , _jump_on_top_damage_observer(shopkeeper)
        {}

        ShopkeeperDeathObserver* get_death_observer()
        {
            return &_death_observer;
        }

        ShopkeeperProjectileDamageObserver* get_projectile_damage_observer()
        {
            return &_projectile_damage_observer;
        }

        ShopkeeperMeleeDamageObserver* get_melee_damage_observer()
        {
            return &_melee_damage_observer;
        }

        ShopkeeperJumpOnTopDamageObserver* get_jump_on_top_damage_observer()
        {
            return &_jump_on_top_damage_observer;
        }

        void update(entt::entity owner, uint32_t delta_time_ms) override;
        void enter_state(ShopkeeperBaseState* new_state, entt::entity owner);

    private:
        ShopkeeperDeathObserver _death_observer;
        ShopkeeperProjectileDamageObserver _projectile_damage_observer;
        ShopkeeperMeleeDamageObserver _melee_damage_observer;
        ShopkeeperJumpOnTopDamageObserver _jump_on_top_damage_observer;

        struct
        {
            ShopkeeperStandingState standing;
            ShopkeeperRunningState running;
            ShopkeeperStunnedState stunned;
            ShopkeeperDeadState dead;
            ShopkeeperHeldDeadState held_dead;
            ShopkeeperFallingState falling;
            ShopkeeperBouncingState bouncing;
            ShopkeeperBaseState* current = &standing;
        } _states;
    };
}
