#pragma once

#include "main-dude/states/MainDudeBaseState.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "entt/entt.hpp"

class MainDudeCliffHangingState : public MainDudeBaseState
{
public:

    MainDudeBaseState* update(MainDudeComponent&, uint32_t delta_time_ms) override;

    void enter(MainDudeComponent&) override;
    void exit(MainDudeComponent&) override;
};
