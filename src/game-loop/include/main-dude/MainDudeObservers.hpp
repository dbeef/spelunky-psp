#pragma once

#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/ClimbingComponent.hpp"
#include "components/damage/TakeFallDamageComponent.hpp"
#include "components/damage/TakeNpcTouchDamageComponent.hpp"
#include "components/damage/HitpointComponent.hpp"

class MainDudeClimbingObserver : Observer<ClimbingEvent>
{
public:
    explicit MainDudeClimbingObserver(entt::entity main_dude) : _main_dude(main_dude) {};
    void on_notify(const ClimbingEvent* event) override;
    ClimbingEvent get_last_event() const { return _last_event; }
private:
    const entt::entity _main_dude;
    ClimbingEvent _last_event{};
};

class MainDudeFallObserver : Observer<FallDamage_t>
{
public:
    explicit MainDudeFallObserver(entt::entity main_dude) : _main_dude(main_dude) {};
    void on_notify(const FallDamage_t * event) override;
private:
    const entt::entity _main_dude;
};

class MainDudeDeathObserver : Observer<DeathEvent>
{
public:
    explicit MainDudeDeathObserver(entt::entity main_dude) : _main_dude(main_dude) {};
    void on_notify(const DeathEvent * event) override;
private:
    const entt::entity _main_dude;
};

class MainDudeNpcDamageObserver : Observer<NpcDamage_t>
{
public:
    explicit MainDudeNpcDamageObserver(entt::entity main_dude) : _main_dude(main_dude) {};
    void on_notify(const NpcDamage_t * event) override;
private:
    const entt::entity _main_dude;
};
