#pragma once

#include "other/ParticleGenerator.hpp"
#include "entt/entt.hpp"

namespace prefabs
{
    class CavemanScript;

    class CavemanBaseState
    {
    public:
        virtual CavemanBaseState* update(CavemanScript&, uint32_t delta_time_ms, entt::entity id) = 0;
        virtual void enter(CavemanScript&, entt::entity id) = 0;
        virtual void exit(CavemanScript&, entt::entity id) = 0;
    };

    class CavemanStandingState : public CavemanBaseState
    {
    public:
        CavemanBaseState* update(CavemanScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(CavemanScript&, entt::entity id) override;
        void exit(CavemanScript&, entt::entity id) override;
        void set_random_standing_timer();
    private:
        int _standing_timer_ms = 0;
    };

    class CavemanRunningState : public CavemanBaseState
    {
    public:
        CavemanBaseState* update(CavemanScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(CavemanScript&, entt::entity id) override;
        void exit(CavemanScript&, entt::entity id) override;
        void set_random_running_timer();
    private:
        int _running_timer_ms = 0;
    };

    class CavemanDeadState : public CavemanBaseState
    {
    public:
        CavemanBaseState* update(CavemanScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(CavemanScript&, entt::entity id) override;
        void exit(CavemanScript&, entt::entity id) override;
    };

    class CavemanHeldDeadState : public CavemanBaseState
    {
    public:
        CavemanBaseState* update(CavemanScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(CavemanScript&, entt::entity id) override;
        void exit(CavemanScript&, entt::entity id) override;
    };

    class CavemanFallingState : public CavemanBaseState
    {
    public:
        CavemanBaseState* update(CavemanScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(CavemanScript&, entt::entity id) override;
        void exit(CavemanScript&, entt::entity id) override;
    };

    class CavemanBouncingState : public CavemanBaseState
    {
    public:
        CavemanBaseState* update(CavemanScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(CavemanScript&, entt::entity id) override;
        void exit(CavemanScript&, entt::entity id) override;
    };

    class CavemanStunnedState : public CavemanBaseState
    {
    public:
        CavemanBaseState* update(CavemanScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(CavemanScript&, entt::entity id) override;
        void exit(CavemanScript&, entt::entity id) override;
    };

    class CavemanHeldStunnedState : public CavemanBaseState
    {
    public:
        CavemanBaseState* update(CavemanScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(CavemanScript&, entt::entity id) override;
        void exit(CavemanScript&, entt::entity id) override;
    };
}
