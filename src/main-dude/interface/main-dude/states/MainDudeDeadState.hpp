#pragma once

#include "main-dude/states/MainDudeBaseState.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"

class MainDudeDeadState : public MainDudeBaseState
{
public:

    MainDudeBaseState* update(MainDudeComponent&, uint32_t delta_time_ms) override;

    void enter(MainDudeComponent&) override;
    void exit(MainDudeComponent&) override;
private:
    void set_current_frame(QuadComponent&, PhysicsComponent&);
};
