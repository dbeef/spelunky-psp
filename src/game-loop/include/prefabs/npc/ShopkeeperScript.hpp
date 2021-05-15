#pragma once

#include "entt/entt.hpp"
#include "prefabs/npc/ShopkeeperStates.hpp"
#include "EntityRegistry.hpp"
#include "components/damage/HitpointComponent.hpp"
#include "components/damage/TakeProjectileDamageComponent.hpp"
#include "components/damage/TakeMeleeDamageComponent.hpp"
#include "components/damage/TakeJumpOnTopDamage.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "system/ShoppingSystem.hpp"

namespace prefabs
{
    class ShopkeeperThieveryObserver final : public Observer<ThieveryEvent>
    {
    public:
        explicit ShopkeeperThieveryObserver(entt::entity shopkeeper) : _shopkeeper(shopkeeper) {}
        void on_notify(const ThieveryEvent*) override;
    private:
        const entt::entity _shopkeeper;
    };

    class ShopkeeperDeathObserver final : public Observer<DeathEvent>
    {
    public:
        explicit ShopkeeperDeathObserver(entt::entity shopkeeper) : _shopkeeper(shopkeeper) {}
        void on_notify(const DeathEvent*) override;
    private:
        const entt::entity _shopkeeper;
    };

    class ShopkeeperProjectileDamageObserver final : public Observer<TakenProjectileDamageEvent>
    {
    public:
        explicit ShopkeeperProjectileDamageObserver(entt::entity shopkeeper) : _shopkeeper(shopkeeper) {}
        void on_notify(const TakenProjectileDamageEvent *) override;
    private:
        const entt::entity _shopkeeper;
    };

    class ShopkeeperMeleeDamageObserver final : public Observer<TakenMeleeDamageEvent>
    {
    public:
        explicit ShopkeeperMeleeDamageObserver(entt::entity shopkeeper) : _shopkeeper(shopkeeper) {}
        void on_notify(const TakenMeleeDamageEvent *) override;
    private:
        const entt::entity _shopkeeper;
    };

    class ShopkeeperJumpOnTopDamageObserver final : public Observer<TakenJumpOnTopDamageEvent>
    {
    public:
        explicit ShopkeeperJumpOnTopDamageObserver(entt::entity shopkeeper) : _shopkeeper(shopkeeper) {}
        void on_notify(const TakenJumpOnTopDamageEvent *) override;
    private:
        const entt::entity _shopkeeper;
    };

    class ShopkeeperScript final : public ScriptBase, public Subject<ShopkeeperAssaultEvent>
    {
    public:
        friend class ShopkeeperDeathObserver;
        friend class ShopkeeperProjectileDamageObserver;
        friend class ShopkeeperJumpOnTopDamageObserver;
        friend class ShopkeeperMeleeDamageObserver;
        friend class ShopkeeperThieveryObserver;

        friend class ShopkeeperStandingState;
        friend class ShopkeeperRunningState;
        friend class ShopkeeperStunnedState;
        friend class ShopkeeperDeadState;
        friend class ShopkeeperHeldDeadState;
        friend class ShopkeeperHeldStunnedState;
        friend class ShopkeeperFallingState;
        friend class ShopkeeperBouncingState;

        explicit ShopkeeperScript(entt::entity shopkeeper)
            : _death_observer(shopkeeper)
            , _projectile_damage_observer(shopkeeper)
            , _melee_damage_observer(shopkeeper)
            , _jump_on_top_damage_observer(shopkeeper)
            , _thievery_observer(shopkeeper)
        {}

        ShopkeeperDeathObserver* get_death_observer() { return &_death_observer; }
        ShopkeeperProjectileDamageObserver* get_projectile_damage_observer() { return &_projectile_damage_observer; }
        ShopkeeperMeleeDamageObserver* get_melee_damage_observer() { return &_melee_damage_observer; }
        ShopkeeperJumpOnTopDamageObserver* get_jump_on_top_damage_observer() { return &_jump_on_top_damage_observer; }
        ShopkeeperThieveryObserver* get_thievery_observer() { return &_thievery_observer; }

        void update(entt::entity owner, uint32_t delta_time_ms) override;
        void enter_state(ShopkeeperBaseState* new_state, entt::entity owner);
        void get_angry(entt::entity shopkeeper);

    private:

        bool _angry = false;
        int _stunned_timer_ms = 0;
        int _horizontal_orientation_switch_timer_ms = 0;

        ShopkeeperDeathObserver _death_observer;
        ShopkeeperProjectileDamageObserver _projectile_damage_observer;
        ShopkeeperMeleeDamageObserver _melee_damage_observer;
        ShopkeeperJumpOnTopDamageObserver _jump_on_top_damage_observer;
        ShopkeeperThieveryObserver _thievery_observer;

        struct
        {
            ShopkeeperStandingState standing;
            ShopkeeperRunningState running;
            ShopkeeperStunnedState stunned;
            ShopkeeperDeadState dead;
            ShopkeeperHeldDeadState held_dead;
            ShopkeeperHeldStunnedState held_stunned;
            ShopkeeperFallingState falling;
            ShopkeeperBouncingState bouncing;
            ShopkeeperBaseState* current = &standing;
        } _states;

        void follow_customer(entt::entity shopkeeper);
        void do_angry_stuff(entt::entity shopkeeper, std::uint32_t delta_time_ms);
    };
}
