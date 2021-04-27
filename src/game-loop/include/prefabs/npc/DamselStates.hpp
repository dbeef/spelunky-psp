#pragma once

#include "other/ParticleGenerator.hpp"
#include "entt/entt.hpp"

namespace prefabs
{
    class DamselScript;

    class DamselBaseState
    {
    public:
        virtual DamselBaseState* update(DamselScript&, uint32_t delta_time_ms, entt::entity id) = 0;
        virtual void enter(entt::entity id) = 0;
        virtual void exit(entt::entity id) = 0;
    };

    class DamselStandingState : public DamselBaseState
    {
    public:
        DamselBaseState* update(DamselScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(entt::entity id) override;
        void exit(entt::entity id) override;
    private:
        int32_t _time_since_last_yell_ms = 0;
    };

    class DamselRunningState : public DamselBaseState
    {
    public:
        DamselBaseState* update(DamselScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(entt::entity id) override;
        void exit(entt::entity id) override;
    };

    class DamselHeldState : public DamselBaseState
    {
    public:
        DamselBaseState* update(DamselScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(entt::entity id) override;
        void exit(entt::entity id) override;
    };

    class DamselExitingState : public DamselBaseState
    {
    public:
        DamselBaseState* update(DamselScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(entt::entity id) override;
        void exit(entt::entity id) override;
    private:
        bool _hidden = false;
    };

    class DamselStunnedState : public DamselBaseState
    {
    public:
        DamselBaseState* update(DamselScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(entt::entity id) override;
        void exit(entt::entity id) override;
    private:
        int32_t _damsel_stunned_timer_ms = 0;
    };

    class DamselYellingState : public DamselBaseState
    {
    public:
        DamselBaseState* update(DamselScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(entt::entity id) override;
        void exit(entt::entity id) override;
    private:
        int32_t _damsel_stunned_timer_ms = 0;
    };

    class DamselDeadState : public DamselBaseState
    {
    public:
        DamselBaseState* update(DamselScript&, uint32_t delta_time_ms, entt::entity id) override;
        void enter(entt::entity id) override;
        void exit(entt::entity id) override;
    private:
        int32_t _damsel_stunned_timer_ms = 0;
    };
}
