#pragma once

#include "other/ParticleGenerator.hpp"
#include "entt/entt.hpp"

namespace prefabs
{
    class ShopkeeperScript;

    class ShopkeeperBaseState
    {
    public:
        virtual ShopkeeperBaseState* update(ShopkeeperScript&, uint32_t delta_time_ms, entt::entity id) = 0;
        virtual void enter(entt::entity id) = 0;
        virtual void exit(entt::entity id) = 0;
    };

    class ShopkeeperStandingState : public ShopkeeperBaseState
    {
    public:
        ShopkeeperBaseState* update(ShopkeeperScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(entt::entity id) override;
        void exit(entt::entity id) override;
    };

    class ShopkeeperRunningState : public ShopkeeperBaseState
    {
    public:
        ShopkeeperBaseState* update(ShopkeeperScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(entt::entity id) override;
        void exit(entt::entity id) override;
    };
}
