#pragma once

#include "entt/entt.hpp"
#include "prefabs/npc/ShopkeeperStates.hpp"
#include "EntityRegistry.hpp"
#include "components/damage/HitpointComponent.hpp"
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

    class ShopkeeperScript final : public ScriptBase
    {
    public:
        friend class ShopkeeperStandingState;
        friend class ShopkeeperRunningState;

        explicit ShopkeeperScript(entt::entity Shopkeeper) : _death_observer(Shopkeeper) {}

        ShopkeeperDeathObserver* get_observer()
        {
            return &_death_observer;
        }

        void update(entt::entity owner, uint32_t delta_time_ms) override;

    private:
        ShopkeeperDeathObserver _death_observer;

        struct
        {
            ShopkeeperStandingState standing;
            ShopkeeperRunningState running;
            ShopkeeperBaseState* current = &standing;
        } _states;
    };
}
