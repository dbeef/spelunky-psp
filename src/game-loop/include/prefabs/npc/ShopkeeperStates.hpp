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
        virtual void enter(ShopkeeperScript&, entt::entity id) = 0;
        virtual void exit(ShopkeeperScript&, entt::entity id) = 0;
    };

    class ShopkeeperStandingState : public ShopkeeperBaseState
    {
    public:
        ShopkeeperBaseState* update(ShopkeeperScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(ShopkeeperScript&, entt::entity id) override;
        void exit(ShopkeeperScript&, entt::entity id) override;
    };

    class ShopkeeperRunningState : public ShopkeeperBaseState
    {
    public:
        ShopkeeperBaseState* update(ShopkeeperScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(ShopkeeperScript&, entt::entity id) override;
        void exit(ShopkeeperScript&, entt::entity id) override;
    };

    class ShopkeeperStunnedState : public ShopkeeperBaseState
    {
    public:
        ShopkeeperBaseState* update(ShopkeeperScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(ShopkeeperScript&, entt::entity id) override;
        void exit(ShopkeeperScript&, entt::entity id) override;
    };

    class ShopkeeperDeadState : public ShopkeeperBaseState
    {
    public:
        ShopkeeperBaseState* update(ShopkeeperScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(ShopkeeperScript&, entt::entity id) override;
        void exit(ShopkeeperScript&, entt::entity id) override;
    };

    class ShopkeeperHeldDeadState : public ShopkeeperBaseState
    {
    public:
        ShopkeeperBaseState* update(ShopkeeperScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(ShopkeeperScript&, entt::entity id) override;
        void exit(ShopkeeperScript&, entt::entity id) override;
    };

    class ShopkeeperHeldStunnedState : public ShopkeeperBaseState
    {
    public:
        ShopkeeperBaseState* update(ShopkeeperScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(ShopkeeperScript&, entt::entity id) override;
        void exit(ShopkeeperScript&, entt::entity id) override;
    };

    class ShopkeeperFallingState : public ShopkeeperBaseState
    {
    public:
        ShopkeeperBaseState* update(ShopkeeperScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(ShopkeeperScript&, entt::entity id) override;
        void exit(ShopkeeperScript&, entt::entity id) override;
    };

    class ShopkeeperBouncingState : public ShopkeeperBaseState
    {
    public:
        ShopkeeperBaseState* update(ShopkeeperScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(ShopkeeperScript&, entt::entity id) override;
        void exit(ShopkeeperScript&, entt::entity id) override;
    };
}
